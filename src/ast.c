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
    dbc(RETURN_VALUE, 0, 0, NULL);
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
        printf("LOAD_NAME %s\n", ret->datos.cadena);
#endif
    }
    break;
    case NODO_ASIGNACION: /*SET*/
    {
        pn(mv, node->l);
        lat_objeto *ret = lat_cadena_nueva(mv, node->r->valor->v.s);
        dbc(STORE_NAME, ret, NULL, NULL);
#if DEPURAR_MV
        printf("STORE_NAME %s\n", ret->datos.cadena);
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
        printf("LOAD_CONST %s\n", ret->datos.literal);
#endif
    }
    break;
    case NODO_ENTERO:
    {
        lat_objeto *ret = lat_entero_nuevo(mv, node->valor->v.i);
        dbc(LOAD_CONST, ret, NULL, NULL);
#if DEPURAR_MV
        printf("LOAD_CONST %ld\n", ret->datos.entero);
#endif
    }
    break;
    case NODO_DECIMAL:
    {
        lat_objeto *ret = lat_decimal_nuevo(mv, node->valor->v.d);
        dbc(LOAD_CONST, ret, NULL, NULL);
#if DEPURAR_MV
        printf("LOAD_CONST %.14g\n", ret->datos.decimal);
#endif
    }
    break;
    case NODO_CADENA:
    {
        lat_objeto *ret = lat_cadena_nueva(mv, node->valor->v.s);
        dbc(LOAD_CONST, ret, NULL, NULL);
#if DEPURAR_MV
        printf("LOAD_CONST %s\n", ret->datos.cadena);
#endif
    }
    break;
    case NODO_LOGICO:
    {
        lat_objeto *ret = node->valor->v.b ? mv->objeto_cierto : mv->objeto_falso;
        dbc(LOAD_CONST, ret, NULL, NULL);
#if DEPURAR_MV
        printf("LOAD_CONST %i\n", ret->datos.logico);
#endif
    }
    break;
    /*case NODO_SI:
    {
        nodo_si *nIf = ((nodo_si *)node);
        pn(mv, nIf->cond);
        dbc(OP_MOV, 2, 255, NULL);
        dbc(OP_MOV, 3, 255, NULL);
        dbc(OP_NOT, 2, 0, NULL);
        temp[0] = i;
        dbc(OP_NOP, 0, 0, NULL);
        pn(mv, nIf->th);
        bcode[temp[0]] = lat_bc(OP_JMPIF, i, 2, NULL);
#if DEPURAR_MV
        printf("MOV R2 R255\n");
        printf("MOV R3 R255\n");
        printf("NOT\n");
        printf("NOP R0 R0\n");
        printf("JMPIF R%i R2\n", i);
#endif
        if (nIf->el)
        {
            temp[1] = i;
            dbc(OP_NOP, 0, 0, NULL);
            pn(mv, nIf->el);
            bcode[temp[1]] = lat_bc(OP_JMPIF, i, 3, NULL);
#if DEPURAR_MV
            printf("NOP\n");
            printf("MOV R3 R255\n");
            printf("JMPIF R%i R3\n", i);
#endif
        }
    }
    break;
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
        //printf("inicio PARAMETROS\n");
        if (nFun->parametros)
        {
            fpn(mv, nFun->parametros);
        }
        //printf("fin PARAMETROS\n");
        //printf("inicio INSTRUCCIONES\n");
        //sentencias de la funcion
        fpn(mv, nFun->sentencias);
        //printf("fin INSTRUCCIONES\n");
        dbc(MAKE_FUNCTION, (void*)funcion_bcode, (void*)num_params, NULL);
        //nombre de la funcion
        //pn(mv, nFun->nombre);
        lat_objeto *ret = lat_cadena_nueva(mv, nFun->nombre->valor->v.s);
        dbc(STORE_NAME, ret, NULL, NULL);
#if DEPURAR_MV
        printf("MAKE_FUNCTION %i\n", num_params);
        printf("STORE_NAME %s\n", nFun->nombre->valor->v.s);
#endif
        funcion_bcode = NULL;
        fi = 0;
        num_params = 0;
        //printf("fin NODO_FUNCION_USUARIO\n");
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
            printf("STORE_NAME %s\n", ret->datos.cadena);
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
        printf("CALL_FUNCTION %i\n", num_args);
#endif
        num_args = 0;
    }
    break;
    case NODO_RETORNO:
    {
        pn(mv, node->l);
        dbc(RETURN_VALUE, NULL, NULL, NULL);
#if DEPURAR_MV
        printf("RETURN_VALUE\n");
#endif
    }
    break;
    case NODO_FUNCION_ARGUMENTOS:
    {
        if (node->l)
        {
            pn(mv, node->l);
            num_args++;
            //dbc(OP_PUSH, 255, 0, NULL);
#if DEPURAR_MV
            //printf("PUSH R255\n");
#endif
        }
        if (node->r)
        {
            pn(mv, node->r);
            if(node->r->valor)
                num_args++;
            //Soporte para recursion NODO_FUNCION_LLAMADA
            //if (node->r->valor || node->r->tipo == NODO_FUNCION_LLAMADA)
            //{
            //    dbc(OP_PUSH, 255, 0, NULL);
#if DEPURAR_MV
            //    printf("PUSH R255\n");
#endif
            //}
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
            dbc(BINARY_ADD, 0, 0, NULL);
#if DEPURAR_MV
        printf("BINARY_ADD\n");
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
            dbc(BINARY_SUBTRACT, 0, 0, NULL);
#if DEPURAR_MV
        printf("BINARY_SUBTRACT\n");
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
            dbc(BINARY_MULTIPLY, 0, 0, NULL);
#if DEPURAR_MV
        printf("BINARY_MULTIPLY\n");
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
            dbc(BINARY_FLOOR_DIVIDE, 0, 0, NULL);
#if DEPURAR_MV
        printf("BINARY_FLOOR_DIVIDE\n");
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
            dbc(BINARY_MODULO, 0, 0, NULL);
#if DEPURAR_MV
        printf("BINARY_MODULO\n");
#endif
        }break;
/*
    case NODO_LISTA:
    {
        nested++;
        //dbc(OP_STORELIST, nested, 0, NULL);
#if DEPURAR_MV
        printf("STORELIST R%i\n", nested);
#endif
        if (node->l)
        {
            pn(mv, node->l);
            dbc(OP_MOV, 255, nested, NULL);
#if DEPURAR_MV
            printf("MOV R255 R%i\n", nested);
#endif
        }
        nested--;
    }
    break;
    case NODO_LISTA_AGREGAR_ELEMENTO:
    {
        if (node->l)
        {
            pn(mv, node->l);
            dbc(OP_PUSHLIST, nested, 255, NULL);
#if DEPURAR_MV
            printf("PUSHLIST R%i R255\n", nested);
#endif
        }
        if (node->r)
        {
            pn(mv, node->r);
        }
    }
    break;
    case NODO_LISTA_ASIGNAR_ELEMENTO:
    {
        nodo_lista_elem *elem = ((nodo_lista_elem *)node);
        if (elem->exp)
        {
            pn(mv, elem->exp);
            dbc(OP_MOV, 3, 255, NULL);
#if DEPURAR_MV
            printf("MOV R3 R255\n");
#endif
        }
        if(elem->id)
        {
            pn(mv, elem->pos);
            dbc(OP_MOV, 4, 255, NULL);
#if DEPURAR_MV
            printf("MOV R4 R255\n");
#endif
            pn(mv, elem->id);
            dbc(OP_LISTSETITEM, 255, 3, (void*)4);
#if DEPURAR_MV
            printf("LISTSETITEM R255 R3\n");
#endif
        }
    }
    break;
    case NODO_LISTA_OBTENER_ELEMENTO:
    {
        if (node->l)
        {
            pn(mv, node->l);
            dbc(OP_MOV, 3, 255, NULL);
#if DEPURAR_MV
            printf("MOV R3 R255\n");
#endif
        }
        if (node->r)
        {
            pn(mv, node->r);
            dbc(OP_LISTGETITEM, 255, 3, NULL);
#if DEPURAR_MV
            printf("LISTGETITEM R55 R3\n");
#endif
        }
    }
    break;
    case NODO_DICCIONARIO:
    {
        nested++;
        dbc(OP_STOREDICT, nested, 0, NULL);
#if DEPURAR_MV
        printf("STOREDICT R%i\n", nested);
#endif
        if (node->l)
        {
            pn(mv, node->l);
            dbc(OP_MOV, 255, nested, NULL);
#if DEPURAR_MV
            printf("MOV R255 R%i\n", nested);
#endif
        }
        nested--;
    }
    break;
    case NODO_DICC_AGREGAR_ELEMENTO:
    {
        if(node->l)
        {
            pn(mv, node->l);
            dbc(OP_PUSHDICT, nested, 255, NULL);
#if DEPURAR_MV
            printf("PUSHDICT R%i R255\n", nested);
#endif
        }
        if(node->r)
        {
            pn(mv, node->r);
        }
    }
    break;
    case NODO_DICC_ELEMENTO:
    {
        if(node->l){
            pn(mv, node->l);
            dbc(OP_PUSH, 255, 0, NULL);
            #if DEPURAR_MV
                printf("PUSH R255\n");
            #endif
        }
        if(node->r){
            pn(mv, node->r);
            dbc(OP_PUSH, 255, 0, NULL);
            #if DEPURAR_MV
                printf("PUSH R255\n");
            #endif
        }
        dbc(OP_PUSHDICTELEM, 0, 0, NULL);
        #if DEPURAR_MV
            printf("OP_PUSHDICTELEM\n");
        #endif
    }
    break;
    case NODO_DICC_ASIGNAR_ELEMENTO:
    {
        nodo_dicc_elem *elem = ((nodo_dicc_elem *)node);
        if(elem->exp)
        {
            pn(mv, elem->exp);
            dbc(OP_MOV, 3, 255, NULL);
#if DEPURAR_MV
            printf("MOV R3 R255\n");
#endif
        }
        if(elem->id)
        {
            pn(mv, elem->llave);
            dbc(OP_MOV, 4, 255, NULL);
#if DEPURAR_MV
            printf("MOV R4 R255\n");
#endif
            pn(mv, elem->id);
            dbc(OP_DICTSETITEM, 255, 3, (void*)4);
#if DEPURAR_MV
            printf("DICTITEM R255 R3\n");
#endif
        }
    }
    break;
    case NODO_DICC_OBTENER_ELEMENTO:
    {
        if(node->l){
            pn(mv, node->l);
            dbc(OP_MOV, 3, 255, NULL);
            #if DEPURAR_MV
            printf("MOV R3 R255\n");
            #endif
        }
        if(node->r){
            pn(mv, node->r);
            dbc(OP_DICTGETITEM, 255, 3, NULL);
            #if DEPURAR_MV
            printf("MOV R255 R3\n");
            #endif
        }
    }break;
*/
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
    // printf("i = %i\n", i);
    return i;
}
