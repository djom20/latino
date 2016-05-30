/*
The MIT License (MIT)

Copyright (c) 2015 - Latino

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include <stdbool.h>
#include "latino.h"
#include "ast.h"
#include "utils.h"
#include "vm.h"
#include "libmem.h"

#define dbc(I, A, B, C) bcode[i++] = lat_bc(I, A, B, C)
#define pn(mv, N) i = nodo_analizar(mv, N, bcode, i)
#define fdbc(I, A, B, C) funcion_bcode[fi++] = lat_bc(I, A, B, C)
#define fpn(mv, N) fi = nodo_analizar(mv, N, funcion_bcode, fi)

ast *nodo_nuevo(nodo_tipo nt, ast *l, ast *r)
{
    ast *a = (ast*)lat_asignar_memoria(sizeof(ast));
    a->tipo = nt;
    a->l = l;
    a->r = r;
    a->valor = NULL;
    return a;
}

ast *nodo_nuevo_entero(long i, int num_linea, int num_columna)
{
    ast *a = (ast*)lat_asignar_memoria(sizeof(ast));
    a->tipo = NODO_ENTERO;
    nodo_valor *val = (nodo_valor*)lat_asignar_memoria(sizeof(nodo_valor));
    val->t = VALOR_ENTERO;
    val->v.i = i;
    a->valor = val;
    a->valor->es_constante = false;
    a->valor->num_linea = num_linea;
    a->valor->num_columna = num_columna;
    return a;
}

ast *nodo_nuevo_decimal(double d, int num_linea, int num_columna)
{
    ast *a = (ast*)lat_asignar_memoria(sizeof(ast));
    a->tipo = NODO_DECIMAL;
    nodo_valor *val = (nodo_valor*)lat_asignar_memoria(sizeof(nodo_valor));
    val->t = VALOR_DECIMAL;
    val->v.d = d;
    a->valor = val;
    a->valor->es_constante = false;
    a->valor->num_linea = num_linea;
    a->valor->num_columna = num_columna;
    return a;
}

ast *nodo_nuevo_logico(int b, int num_linea, int num_columna)
{
    ast *a = (ast*)lat_asignar_memoria(sizeof(ast));
    a->tipo = NODO_LOGICO;
    nodo_valor *val = (nodo_valor*)lat_asignar_memoria(sizeof(nodo_valor));
    val->t = VALOR_LOGICO;
    val->v.b = b;
    a->valor = val;
    a->valor->es_constante = false;
    a->valor->num_linea = num_linea;
    a->valor->num_columna = num_columna;
    return a;
}

ast *nodo_nuevo_literal(char *c, int num_linea, int num_columna)
{
    ast *a = (ast*)lat_asignar_memoria(sizeof(ast));
    a->tipo = NODO_LITERAL;
    nodo_valor *val = (nodo_valor*)lat_asignar_memoria(sizeof(nodo_valor));
    val->t = VALOR_LITERAL;
    a->valor = val;
    a->valor->v.c = parse_string(c, strlen(c));
    a->valor->es_constante = false;
    a->valor->num_linea = num_linea;
    a->valor->num_columna = num_columna;
    return a;
}

ast *nodo_nuevo_cadena(const char *s, int num_linea, int num_columna)
{
    ast *a = (ast*)lat_asignar_memoria(sizeof(ast));
    a->tipo = NODO_CADENA;
    nodo_valor *val = (nodo_valor*)lat_asignar_memoria(sizeof(nodo_valor));
    val->t = VALOR_CADENA;
    val->v.s = parse_string(s, strlen(s));
    a->valor = val;
    a->valor->es_constante = false;
    a->valor->num_linea = num_linea;
    a->valor->num_columna = num_columna;
    return a;
}

ast *nodo_nuevo_constante(char *s, int num_linea, int num_columna)
{
    ast *a = (ast*)lat_asignar_memoria(sizeof(ast));
    a->tipo = NODO_IDENTIFICADOR;
    nodo_valor *val = (nodo_valor*)lat_asignar_memoria(sizeof(nodo_valor));
    val->t = VALOR_CADENA;
    val->v.s = strdup0(s);
    a->valor = val;
    a->valor->es_constante = true;
    a->valor->num_linea = num_linea;
    a->valor->num_columna = num_columna;
    return a;
}

ast *nodo_nuevo_identificador(const char *s, int num_linea, int num_columna)
{
    ast *a = (ast*)lat_asignar_memoria(sizeof(ast));
    a->tipo = NODO_IDENTIFICADOR;
    nodo_valor *val = (nodo_valor*)lat_asignar_memoria(sizeof(nodo_valor));
    val->t = VALOR_CADENA;
    val->v.s = strdup0(s);
    a->valor = val;
    a->valor->es_constante = false;
    a->valor->num_linea = num_linea;
    a->valor->num_columna = num_columna;
    return a;
}

ast *nodo_nuevo_asignacion(ast *v, ast *s)
{
    ast *a = (ast*)lat_asignar_memoria(sizeof(ast));
    a->tipo = NODO_ASIGNACION;
    a->l = v;
    a->r = s;
    a->valor = NULL;
    return a;
}

ast *nodo_nuevo_asignacion_lista_elem(ast *exp, ast *id, ast *pos)
{
    nodo_lista_elem *a = (nodo_lista_elem*)lat_asignar_memoria(sizeof(nodo_lista_elem));
    a->tipo = NODO_LISTA_ASIGNAR_ELEMENTO;
    a->exp = exp;
    a->id = id;
    a->pos = pos;
    return (ast *)a;
}

ast *nodo_nuevo_asignacion_dicc_elem(ast *exp, ast *id, ast *llave)
{
    nodo_lista_elem *a = (nodo_lista_elem*)lat_asignar_memoria(sizeof(nodo_lista_elem));
    a->tipo = NODO_DICC_ASIGNAR_ELEMENTO;
    a->exp = exp;
    a->id = id;
    a->pos = llave;
    return (ast *)a;
}

ast *nodo_nuevo_si(ast *cond, ast *th, ast *el)
{
    nodo_si *a = (nodo_si*)lat_asignar_memoria(sizeof(nodo_si));
    a->tipo = NODO_SI;
    a->cond = cond;
    a->th = th;
    a->el = el;
    return (ast *)a;
}

ast *nodo_nuevo_mientras(ast *cond, ast *stmts)
{
    ast *a = (ast*)lat_asignar_memoria(sizeof(ast));
    a->tipo = NODO_MIENTRAS;
    a->l = cond;
    a->r = stmts;
    a->valor = NULL;
    return a;
}

ast *nodo_nuevo_hacer(ast *cond, ast *stmts)
{
    ast *a = (ast*)lat_asignar_memoria(sizeof(ast));
    a->tipo = NODO_HACER;
    a->l = cond;
    a->r = stmts;
    a->valor = NULL;
    return a;
}

ast *nodo_nuevo_desde(ast *dec, ast *cond, ast *inc, ast *stmts)
{
    ast *a = (ast*)lat_asignar_memoria(sizeof(ast));
    a->tipo = NODO_BLOQUE;
    a->l = nodo_nuevo_mientras(cond, nodo_nuevo(NODO_BLOQUE, inc, stmts));
    a->r = dec;
    a->valor = NULL;
    return a;
}

ast *nodo_nuevo_funcion(ast *nombre, ast *parametros, ast *sentencias)
{
    nodo_funcion *f = (nodo_funcion*)lat_asignar_memoria(sizeof(nodo_funcion));
    f->tipo = NODO_FUNCION_USUARIO;
    f->nombre = nombre;
    f->parametros = parametros;
    f->sentencias = sentencias;
    return (ast *)f;
}

ast* nodo_nuevo_incluir(ast* ruta)
{
    ast *a = (ast*)lat_asignar_memoria(sizeof(ast));
    a->tipo = NODO_INCLUIR;
    a->l = ruta;
    a->r = NULL;
    a->valor = NULL;
    return a;
}

void nodo_liberar(ast *a)
{
    if (a)
    {
        switch (a->tipo)
        {
        case NODO_BLOQUE:
        case NODO_LISTA_AGREGAR_ELEMENTO:
            if (a->r)
                nodo_liberar(a->r);
            if (a->l)
                nodo_liberar(a->l);
            break;
        default:
            if (a->tipo)
                lat_liberar_memoria(a->valor);
            lat_liberar_memoria(a);
            break;
        }
    }
}

lat_objeto *nodo_analizar_arbol(lat_mv *mv, ast *tree)
{
    lat_bytecode *bcode = (lat_bytecode *)lat_asignar_memoria(sizeof(lat_bytecode) * MAX_BYTECODE_FUNCTION);
    int i = nodo_analizar(mv, tree, bcode, 0);
    dbc(RETURN_VALUE, NULL, NULL, NULL);
    nodo_liberar(tree);
    return lat_definir_funcion(mv, bcode, 0);
}

int nested = -1;
int num_params = 0;
int num_args = 0;

int nodo_analizar(lat_mv *mv, ast *node, lat_bytecode *bcode, int i)
{
    int temp[8] = {0};
    lat_bytecode *funcion_bcode = NULL;
    int fi = 0;
    switch (node->tipo)
    {
    /*
    case NODO_INCLUIR:
    {
        //TODO: Incluir rutas con punto ej. incluir "lib.modulos.myModulo"
        char* archivo = node->l->valor->v.s;
        lat_objeto* mod = lat_cadena_nueva(mv, archivo);
        if(!find_list(mv->modulos, (void*)mod))
        {
            //encontrar el modulo en la ruta actual
            char dir_actual[MAX_PATH_LENGTH];
            ldirectorio_actual(dir_actual, sizeof(dir_actual));
            strcat(dir_actual, PATH_SEP);
            strcat(dir_actual, archivo);
            if(!endsWith(dir_actual, ".lat"))
            {
                strcat(dir_actual, ".lat");
            }
            if(legible(dir_actual))
            {
                insert_list(mv->modulos, mod);
                pn(mv, lat_analizar_archivo(mv, dir_actual));
            }
            else
            {
                //sino existe buscar en el path_home de latino/lib
                char* latino_lib = getenv("LATINO_LIB");
                if(latino_lib != NULL)
                {
                    strcat(latino_lib, PATH_SEP);
                    strcat(latino_lib, archivo);
                    if(!endsWith(latino_lib, ".lat"))
                    {
                        strcat(latino_lib, ".lat");
                    }
                    if(legible(latino_lib))
                    {
                        insert_list(mv->modulos, mod);
                        pn(mv, lat_analizar_archivo(mv, latino_lib));
                    }
                }
            }
        }
        //return NULL;
    }
    break;
    */
    case NODO_BLOQUE:
    {
        if (node->r)
        {
            pn(mv, node->r);
        }
        if (node->l)
        {
            pn(mv, node->l);
        }
    }
    break;
    case NODO_IDENTIFICADOR: /*GET*/
    {
        lat_objeto *ret = lat_cadena_nueva(mv, node->valor->v.s);
        dbc(LOAD_NAME, ret, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tLOAD_NAME %s\n", i, ret->datos.cadena);
#endif
    }
    break;
    case NODO_ASIGNACION: /*SET*/
    {
        pn(mv, node->l);
        lat_objeto *ret = lat_cadena_nueva(mv, node->r->valor->v.s);
        dbc(STORE_NAME, ret, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tSTORE_NAME %s\n", i, ret->datos.cadena);
#endif
    }
    break;
    /*
    case NODO_INCREMENTO:
    {
        pn(mv, node->l);
        dbc(OP_INC, 255, 0, NULL);
#if DEPURAR_MV
        printf("INC R255 R0\n");
#endif
    }
    break;
    case NODO_DECREMENTO:
    {
        pn(mv, node->l);
        dbc(OP_DEC, 255, 0, NULL);
#if DEPURAR_MV
        printf("DEC R255 R0\n");
#endif
    }
    break;
    */
    case NODO_LITERAL:
    {
        lat_objeto *ret = lat_literal_nuevo(mv, node->valor->v.c);
        dbc(LOAD_CONST, ret, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tLOAD_CONST %s\n", i, ret->datos.literal);
#endif
    }
    break;
    case NODO_ENTERO:
    {
        lat_objeto *ret = lat_entero_nuevo(mv, node->valor->v.i);
        dbc(LOAD_CONST, ret, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tLOAD_CONST %ld\n", i, ret->datos.entero);
#endif
    }
    break;
    case NODO_DECIMAL:
    {
        lat_objeto *ret = lat_decimal_nuevo(mv, node->valor->v.d);
        dbc(LOAD_CONST, ret, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tLOAD_CONST %.14g\n", i, ret->datos.decimal);
#endif
    }
    break;
    case NODO_CADENA:
    {
        lat_objeto *ret = lat_cadena_nueva(mv, node->valor->v.s);
        dbc(LOAD_CONST, ret, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tLOAD_CONST %s\n", i, ret->datos.cadena);
#endif
    }
    break;
    case NODO_LOGICO:
    {
        lat_objeto *ret = node->valor->v.b ? mv->objeto_cierto : mv->objeto_falso;
        dbc(LOAD_CONST, ret, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tLOAD_CONST %i\n", i, ret->datos.logico);
#endif
    }
    break;
    case NODO_SI:
    {
        nodo_si *nIf = ((nodo_si *)node);
        pn(mv, nIf->cond);
        temp[0] = i;
        /*instruccion auxiliar para suplantar por jump_if_true */
        dbc(NOP, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tNOP\n", i);
#endif
        pn(mv, nIf->th);
        bcode[temp[0]] = lat_bc(POP_JUMP_IF_FALSE, (void*)i, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tPOP_JUMP_IF_FALSE %i\n", i, i);
#endif
        if (nIf->el)
        {
            pn(mv, nIf->cond);  //TODO: Quitar esta evaluacion
            temp[1] = i;
            /*instruccion auxiliar para suplantar por jump_if_false */
            dbc(NOP, NULL, NULL, NULL);
#if DEPURAR_MV
            printf("%i\tNOP\n", i);
#endif
            pn(mv, nIf->el);
            bcode[temp[1]] = lat_bc(POP_JUMP_IF_TRUE, (void*)i, NULL, NULL);
#if DEPURAR_MV            
            printf("%i\tPOP_JUMP_IF_TRUE %i\n", i, i);
#endif
        }
    }
    break;
    case NODO_MIENTRAS:
    {
        temp[0] = i;
        pn(mv, node->l);
        temp[1] = i;
        dbc(NOP, NULL, NULL, NULL);
        pn(mv, node->r);
        dbc(POP_JUMP_IF_FALSE, (void*)temp[0], NULL, NULL);
        //bcode[temp[1]] = lat_bc(POP_JUMP_IF_FALSE, i, NULL, NULL);
    }
    break;
    
    /*
    case NODO_MIENTRAS:
    {
        temp[0] = i;
        pn(mv, node->l);
        dbc(OP_MOV, 2, 255, NULL);
        dbc(OP_NOT, 2, 0, NULL);
        temp[1] = i;
        dbc(OP_NOP, 0, 0, NULL);
        pn(mv, node->r);
        dbc(OP_JMP, temp[0], 0, NULL);
        bcode[temp[1]] = lat_bc(OP_JMPIF, i, 2, NULL);
#if DEPURAR_MV
        printf("MOV R2 R255\n");
        printf("NOT R2 R0\n");
        printf("NOP\n");
        printf("JMP R%i\n", temp[0]);
        printf("JMPIF R%i R2\n", i);
#endif
    }
    break;
    case NODO_HACER:
    {
        pn(mv, node->r);
        temp[0] = i;
        pn(mv, node->l);
        dbc(OP_MOV, 2, 255, NULL);
        dbc(OP_NOT, 2, 0, NULL);
        temp[1] = i;
        dbc(OP_NOP, 0, 0, NULL);
        pn(mv, node->r);
        dbc(OP_JMP, temp[0], 0, NULL);
        bcode[temp[1]] = lat_bc(OP_JMPIF, i, 2, NULL);
#if DEPURAR_MV
        printf("MOV R2 R255\n");
        printf("NOT R2 R0\n");
        printf("NOP\n");
        printf("JMP R%i\n", temp[0]);
        printf("JMPIF R%i R2\n", i);
#endif
    }
    break;
    */
    case NODO_FUNCION_USUARIO:
    {
        nodo_funcion *nFun = ((nodo_funcion *)node);
        funcion_bcode =
            (lat_bytecode *)lat_asignar_memoria(sizeof(lat_bytecode) * MAX_BYTECODE_FUNCTION);
        fi = 0;
        num_params = 0;
        //parametros de la funcion
        if (nFun->parametros)
        {
            fpn(mv, nFun->parametros);
        }        
        fpn(mv, nFun->sentencias);        
        dbc(MAKE_FUNCTION, (void*)funcion_bcode, (void*)num_params, NULL);        
        lat_objeto *ret = lat_cadena_nueva(mv, nFun->nombre->valor->v.s);
        dbc(STORE_NAME, ret, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tMAKE_FUNCTION %i\n", i, num_params);
        printf("%i\tSTORE_NAME %s\n", i, nFun->nombre->valor->v.s);
#endif
        funcion_bcode = NULL;
        fi = 0;
        num_params = 0;        
    }
    break;
    case NODO_LISTA_PARAMETROS:
    {
        lat_objeto* ret = NULL;
        if (node->l)
        {
            if(node->l->valor){
                ret = lat_clonar_objeto(mv, lat_cadena_nueva(mv, node->l->valor->v.s));
                dbc(STORE_NAME, ret, NULL, NULL);
            }
            //pn(mv, node->l);
            num_params++;
#if DEPURAR_MV
            printf("%i\tSTORE_NAME %s\n", i, ret->datos.cadena);
#endif
        }
        if (node->r)
            pn(mv, node->r);
    }
    break;
    case NODO_FUNCION_LLAMADA:
    {
        //procesa los argumentos
        num_args = 0;
        if (node->r)
        {
            pn(mv, node->r);
        }
        //procesa el identificador de la funcion ej. escribir
        pn(mv, node->l);
        dbc(CALL_FUNCTION, (void*)num_args, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tCALL_FUNCTION %i\n", i, num_args);
#endif
        num_args = 0;
    }
    break;
    case NODO_RETORNO:
    {
        pn(mv, node->l);
        dbc(RETURN_VALUE, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tRETURN_VALUE\n", i);
#endif
    }
    break;
    case NODO_FUNCION_ARGUMENTOS:
    {
        if (node->l)
        {
            pn(mv, node->l);
            num_args++;
        }
        if (node->r)
        {
            pn(mv, node->r);
            if(node->r->valor)
                num_args++;
        }
    }
    break;
    case NODO_SUMA:
        {
            if(node->l){
                pn(mv, node->l);
            }
            if(node->r){
                pn(mv, node->r);
            }
            dbc(BINARY_ADD, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tBINARY_ADD\n", i);
#endif
        }break;
    case NODO_RESTA:
        {
            if(node->l){
                pn(mv, node->l);
            }
            if(node->r){
                pn(mv, node->r);
            }
            dbc(BINARY_SUBTRACT, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tBINARY_SUBTRACT\n", i);
#endif
        }break;
    case NODO_MULTIPLICACION:
        {
            if(node->l){
                pn(mv, node->l);
            }
            if(node->r){
                pn(mv, node->r);
            }
            dbc(BINARY_MULTIPLY, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tBINARY_MULTIPLY\n", i);
#endif
        }break;
    case NODO_DIVISION:
        {
            if(node->l){
                pn(mv, node->l);
            }
            if(node->r){
                pn(mv, node->r);
            }
            dbc(BINARY_FLOOR_DIVIDE, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tBINARY_FLOOR_DIVIDE\n", i);
#endif
        }break;
    case NODO_MODULO:
        {
            if(node->l){
                pn(mv, node->l);
            }
            if(node->r){
                pn(mv, node->r);
            }
            dbc(BINARY_MODULO, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tBINARY_MODULO\n", i);
#endif
        }break;
    case NODO_MENOR_QUE:
        {
            if(node->l){
                pn(mv, node->l);
            }
            if(node->r){
                pn(mv, node->r);
            }
            dbc(COMPARE_OP_LT, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("%i\tCOMPARE_OP_LT\n", i);
#endif
        }break;
    case NODO_MENOR_IGUAL:
        {
            if(node->l){
                pn(mv, node->l);
            }
            if(node->r){
                pn(mv, node->r);
            }
            dbc(COMPARE_OP_LTE, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("%i\t\COMPARE_OP_LTE\n", i);
#endif
        }break;
    case NODO_MAYOR_QUE:
        {
            if(node->l){
                pn(mv, node->l);
            }
            if(node->r){
                pn(mv, node->r);
            }
            dbc(COMPARE_OP_GT, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("%i\t\COMPARE_OP_GT\n", i);
#endif
        }break;
    case NODO_MAYOR_IGUAL:
        {
            if(node->l){
                pn(mv, node->l);
            }
            if(node->r){
                pn(mv, node->r);
            }
            dbc(COMPARE_OP_GTE, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("%i\t\COMPARE_OP_GTE\n", i);
#endif
        }break;
    case NODO_IGUALDAD:
        {
            if(node->l){
                pn(mv, node->l);
            }
            if(node->r){
                pn(mv, node->r);
            }
            dbc(COMPARE_OP_EQ, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("%i\t\COMPARE_OP_EQ\n", i);
#endif
        }break;
    case NODO_DESIGUALDAD:
        {
            if(node->l){
                pn(mv, node->l);
            }
            if(node->r){
                pn(mv, node->r);
            }
            dbc(COMPARE_OP_NEQ, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("%i\t\COMPARE_OP_NEQ\n", i);
#endif
        }break;
    /*case NS:
      {
          dbc(OP_NS, 255, 0, NULL);
          pn(node->l);
          dbc(RETURN_VALUENS, 255, 0, NULL);
      }
      break;*/
    default:
        printf("nodo_tipo:%i\n", node->tipo);
        return 0;
    }
    //printf("i = %i\n", i);
    return i;
}
