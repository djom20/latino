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
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "vm.h"
#include "utils.h"
#include "libmem.h"
#include "libmath.h"
#include "libstring.h"
#include "libio.h"

lat_mv* lat_crear_maquina_virtual()
{
    lat_mv* ret = (lat_mv*)lat_asignar_memoria(sizeof(lat_mv));
    ret->pila = lat_crear_lista();
    ret->todos_objetos = lat_crear_lista();
    ret->basurero_objetos = lat_crear_lista();
    ret->modulos = lat_crear_lista();
    ret->memoria_usada = 0;
    ret->objeto_cierto = lat_logico_nuevo(ret, true);
    ret->objeto_falso = lat_logico_nuevo(ret, false);
    //memset(ret->registros, 0, 256);
    memset(ret->contexto_pila, 0, 256);
    ret->contexto_pila[0] = lat_instancia(ret);
    ret->apuntador_pila = 0;
    //lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "+"), lat_definir_cfuncion(ret, lat_sumar));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "-"), lat_definir_cfuncion(ret, lat_restar));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "*"), lat_definir_cfuncion(ret, lat_multiplicar));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "/"), lat_definir_cfuncion(ret, lat_dividir));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "%"), lat_definir_cfuncion(ret, lat_modulo));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "!="), lat_definir_cfuncion(ret, lat_diferente));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "=="), lat_definir_cfuncion(ret, lat_igualdad));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "<"), lat_definir_cfuncion(ret, lat_menor_que));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "<="), lat_definir_cfuncion(ret, lat_menor_igual));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, ">"), lat_definir_cfuncion(ret, lat_mayor_que));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, ">="), lat_definir_cfuncion(ret, lat_mayor_igual));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "&&"), lat_definir_cfuncion(ret, lat_y));
    //lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "y"), lat_definir_cfuncion(ret, lat_y));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "||"), lat_definir_cfuncion(ret, lat_o));
    //lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "o"), lat_definir_cfuncion(ret, lat_o));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "!"), lat_definir_cfuncion(ret, lat_negacion));
    //lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "no"), lat_definir_cfuncion(ret, lat_negacion));
    //lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "gc"), lat_definir_cfuncion(ret, lat_basurero));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "imprimir"), lat_definir_cfuncion(ret, lat_imprimir));

    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "ejecutar"), lat_definir_cfuncion(ret, lat_ejecutar));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "ejecutar_archivo"), lat_definir_cfuncion(ret, lat_ejecutar_archivo));

    /* funciones matematicas */
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "arco_coseno"), lat_definir_cfuncion(ret, lat_arco_coseno));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "arco_seno"), lat_definir_cfuncion(ret, lat_arco_seno));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "arco_tangente"), lat_definir_cfuncion(ret, lat_arco_tangente));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "arco_tangente2"), lat_definir_cfuncion(ret, lat_arco_tangente2));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "coseno"), lat_definir_cfuncion(ret, lat_coseno));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "coseno_hiperbolico"), lat_definir_cfuncion(ret, lat_coseno_hiperbolico));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "seno"), lat_definir_cfuncion(ret, lat_seno));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "seno_hiperbolico"), lat_definir_cfuncion(ret, lat_seno_hiperbolico));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "tangente"), lat_definir_cfuncion(ret, lat_tangente));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "tangente_hiperbolica"), lat_definir_cfuncion(ret, lat_tangente_hiperbolica));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "exponente"), lat_definir_cfuncion(ret, lat_exponente));
    //lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "frexp"), lat_definir_cfuncion(ret, lat_frexp));
    //lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "ldexp"), lat_definir_cfuncion(ret, lat_ldexp));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "logaritmo_natural"), lat_definir_cfuncion(ret, lat_logaritmo_natural));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "logaritmo_base10"), lat_definir_cfuncion(ret, lat_logaritmo_base10));
    //lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "modf"), lat_definir_cfuncion(ret, lat_modf));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "potencia"), lat_definir_cfuncion(ret, lat_potencia));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "raiz_cuadrada"), lat_definir_cfuncion(ret, lat_raiz_cuadrada));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "redondear_arriba"), lat_definir_cfuncion(ret, lat_redondear_arriba));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "valor_absoluto"), lat_definir_cfuncion(ret, lat_valor_absoluto));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "redondear_abajo"), lat_definir_cfuncion(ret, lat_redondeo_abajo));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "modulo"), lat_definir_cfuncion(ret, lat_modulo_decimal));

    /*funciones para cadenas (string)*/
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "comparar"), lat_definir_cfuncion(ret, lat_comparar));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "concatenar"), lat_definir_cfuncion(ret, lat_concatenar));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "."), lat_definir_cfuncion(ret, lat_concatenar));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "contiene"), lat_definir_cfuncion(ret, lat_contiene));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "copiar"), lat_definir_cfuncion(ret, lat_copiar));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "termina_con"), lat_definir_cfuncion(ret, lat_termina_con));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "es_igual"), lat_definir_cfuncion(ret, lat_es_igual));
    //TODO: Pendiente
    //lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "formato"), lat_definir_cfuncion(ret, lat_format));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "indice"), lat_definir_cfuncion(ret, lat_indice));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "insertar"), lat_definir_cfuncion(ret, lat_insertar));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "ultimo_indice"), lat_definir_cfuncion(ret, lat_ultimo_indice));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "rellenar_izquierda"), lat_definir_cfuncion(ret, lat_rellenar_izquierda));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "rellenar_derecha"), lat_definir_cfuncion(ret, lat_rellenar_derecha));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "eliminar"), lat_definir_cfuncion(ret, lat_eliminar));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "esta_vacia"), lat_definir_cfuncion(ret, lat_esta_vacia));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "longitud"), lat_definir_cfuncion(ret, lat_longitud));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "reemplazar"), lat_definir_cfuncion(ret, lat_reemplazar));
    //TODO: Pendiente, se implementara cuando se implementen Listas
    //lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "partir"), lat_definir_cfuncion(ret, lat_split));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "empieza_con"), lat_definir_cfuncion(ret, lat_empieza_con));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "subcadena"), lat_definir_cfuncion(ret, lat_subcadena));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "minusculas"), lat_definir_cfuncion(ret, lat_minusculas));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "mayusculas"), lat_definir_cfuncion(ret, lat_mayusculas));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "quitar_espacios"), lat_definir_cfuncion(ret, lat_quitar_espacios));

    //entrada / salida
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "leer"), lat_definir_cfuncion(ret, lat_leer));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "escribir"), lat_definir_cfuncion(ret, lat_imprimir));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "leer_archivo"), lat_definir_cfuncion(ret, lat_leer_archivo));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "escribir_archivo"), lat_definir_cfuncion(ret, lat_escribir_archivo));

    /*conversion de tipos de dato*/
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "tipo"), lat_definir_cfuncion(ret, lat_tipo));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "logico"), lat_definir_cfuncion(ret, lat_logico));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "entero"), lat_definir_cfuncion(ret, lat_entero));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "decimal"), lat_definir_cfuncion(ret, lat_decimal));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "cadena"), lat_definir_cfuncion(ret, lat_cadena));
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "salir"), lat_definir_cfuncion(ret, lat_salir));

    /*ejemplo de implementacion de una funcion en C */
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "maximo"), lat_definir_cfuncion(ret, lat_maximo));
    /*Creacion dela funcion minimo*/
    lat_asignar_contexto_objeto(lat_obtener_contexto(ret), lat_cadena_nueva(ret, "minimo"), lat_definir_cfuncion(ret, lat_minimo));

    return ret;
}

void lat_apilar(lat_mv *mv, lat_objeto* o)
{
    insert_list(mv->pila, (void*)o);
}

lat_objeto* lat_desapilar(lat_mv *mv)
{
    list_node* n = mv->pila->next;
    if (n->data == NULL)
    {
        lat_registrar_error("Pila vacia");
    }
    else
    {
        n->prev->next = n->next;
        n->next->prev = n->prev;
        lat_objeto* ret = (lat_objeto*)n->data;
        lat_liberar_memoria(n);
        return ret;
    }
    return NULL;
}

void lat_apilar_lista(lat_objeto* lista, lat_objeto* o)
{
    insert_list(lista->datos.lista, (void*)o);
}

lat_objeto* lat_desapilar_lista(lat_objeto* lista)
{
    list_node* n = ((list_node*)lista)->next;
    if (n->data == NULL)
    {
        lat_registrar_error("Lista vacia");
    }
    else
    {
        n->prev->next = n->next;
        n->next->prev = n->prev;
        lat_objeto* ret = (lat_objeto*)n->data;
        return ret;
    }
    return NULL;
}

void lat_apilar_contexto(lat_mv *mv)
{
    if (mv->apuntador_pila >= MAX_STACK_SIZE)
    {
        lat_registrar_error("Namespace desborde de la pila");
    }
    mv->contexto_pila[mv->apuntador_pila + 1] = lat_clonar_objeto(mv, mv->contexto_pila[mv->apuntador_pila]);
    mv->apuntador_pila++;
}

void lat_desapilar_contexto(lat_mv *mv)
{
    if (mv->apuntador_pila == 0)
    {
        lat_registrar_error("Namespace pila vacia");
    }
    lat_eliminar_objeto(mv, mv->contexto_pila[mv->apuntador_pila--]);
}

void lat_apilar_contexto_predefinido(lat_mv *mv, lat_objeto* ctx)
{
    if (mv->apuntador_pila >= 255)
    {
        lat_registrar_error("Namespace desborde de la pila");
    }
    mv->contexto_pila[++mv->apuntador_pila] = ctx;
}

lat_objeto* lat_desapilar_contexto_predefinido(lat_mv *mv)
{
    if (mv->apuntador_pila == 0)
    {
        lat_registrar_error("Namespace pila vacia");
    }
    return mv->contexto_pila[mv->apuntador_pila--];
}

lat_objeto* lat_obtener_contexto(lat_mv *mv)
{
    return mv->contexto_pila[mv->apuntador_pila];
}

void lat_basurero_agregar(lat_mv *mv, lat_objeto* o)
{
    insert_list(mv->basurero_objetos, (void*)o);
}

void lat_basurero(lat_mv *mv)
{
    int i = 0;
    /*for (i = 0; i < 256; i++)
    {
        if (((lat_objeto*)mv->registros[i]) != 0x0)
        {
            if (((lat_objeto*)mv->registros[i])->marcado != 3)
            {
                ((lat_objeto*)mv->registros[i])->marcado = 2;
            }
        }
    }*/
    list_node* c;
    lat_objeto* cur;
    for (c = mv->basurero_objetos->next; c != NULL; c = c->next)
    {
        if (c->data != NULL)
        {
            cur = (lat_objeto*)c->data;
            if (cur->marcado == 0)
            {
                lat_eliminar_objeto(mv, cur);
                list_node* prev = c->prev;
                c->prev->next = c->next;
                c->next->prev = c->prev;
                //free(c);
                c = prev;
            }
            else if (cur->marcado == 2)
            {
                cur->marcado = 1;
            }
            else if (cur->marcado == 1)
            {
                cur->marcado = 0;
            }
        }
        //free(c);
    }
}

lat_objeto* lat_definir_funcion(lat_mv *mv, lat_bytecode* inslist, int num_params)
{
    lat_objeto* ret = lat_funcion_nueva(mv);
    lat_funcion* fval = (lat_funcion*)lat_asignar_memoria(sizeof(lat_funcion));
    fval->bcode = inslist;
    fval->num_params = num_params;
    ret->datos.funcion = fval;
    //mv->memoria_usada += sizeof(sizeof(lat_function));
    return ret;
}

lat_objeto* lat_definir_cfuncion(lat_mv *mv, void (*function)(lat_mv *mv))
{
    lat_objeto* ret = lat_cfuncion_nueva(mv);
    ret->datos.cfunc = function;
    return ret;
}

void lat_numero_lista(lat_mv *mv)
{
    lat_objeto* index = lat_desapilar(mv);
    long i = lat_obtener_entero(index);
    lat_objeto* lista = lat_desapilar(mv);
    list_node* l = lista->datos.lista;
    int counter = 0;
    list_node* c;
    for (c = l->next; c->next != NULL; c = c->next)
    {
        if (c->data != NULL)
        {
            if (counter == i)
            {
                lat_apilar(mv, (lat_objeto*)c->data);
                return;
            }
            counter++;
        }
    }
    lat_registrar_error("Lista: indice fuera de rango");
}

static void lat_imprimir_elem(lat_mv *mv)
{
    lat_objeto* in = lat_desapilar(mv);
    if (in->tipo == T_NULO)
    {
        fprintf(stdout, "%s", "nulo");
    }
    else if (in->tipo == T_INSTANCE)
    {
        fprintf(stdout, "%s", "objeto");
    }
    else if (in->tipo == T_LIT)
    {
        fprintf(stdout, "%s", lat_obtener_literal(in));
    }
    else if (in->tipo == T_INT)
    {
        fprintf(stdout, "%ld", lat_obtener_entero(in));
    }
    else if (in->tipo == T_DOUBLE)
    {
        fprintf(stdout, "%.14g\n", lat_obtener_decimal(in));
    }
    else if (in->tipo == T_STR)
    {
        fprintf(stdout, "%s", lat_obtener_cadena(in));
    }
    else if (in->tipo == T_BOOL)
    {
        fprintf(stdout, "%i", lat_obtener_logico(in));
    }
    else if (in->tipo == T_LIST)
    {
        lat_imprimir_lista(mv, in->datos.lista);
    }
    else if (in->tipo == T_DICT)
    {
        lat_imprimir_diccionario(mv, in->datos.diccionario);
    }
    else if (in->tipo == T_FUNC)
    {
        fprintf(stdout, "%s", "Funcion");
    }
    else if (in->tipo == T_CFUNC)
    {
        fprintf(stdout, "%s", "C_Funcion");
    }
    else if (in->tipo == T_STRUCT)
    {
        fprintf(stdout, "%s", "Struct");
    }
    else
    {
        fprintf(stdout, "Tipo desconocido %d\n", in->tipo);
    }
}

void lat_imprimir(lat_mv *mv)
{
    lat_objeto* in = lat_desapilar(mv);
    if (in->tipo == T_NULO)
    {
        fprintf(stdout, "%s\n", "nulo");
    }
    else if (in->tipo == T_INSTANCE)
    {
        fprintf(stdout, "%s\n", "Objeto");
    }
    else if (in->tipo == T_LIT)
    {
        fprintf(stdout, "%s\n", lat_obtener_literal(in));
    }
    else if (in->tipo == T_INT)
    {
        fprintf(stdout, "%ld\n", lat_obtener_entero(in));
    }
    else if (in->tipo == T_DOUBLE)
    {
        fprintf(stdout, "%.14g\n", lat_obtener_decimal(in));
    }
    else if (in->tipo == T_STR)
    {
        fprintf(stdout, "%s\n", lat_obtener_cadena(in));
    }
    else if (in->tipo == T_BOOL)
    {
        if (lat_obtener_logico(in))
        {
            fprintf(stdout, "%s\n", "verdadero");
        }
        else
        {
            fprintf(stdout, "%s\n", "falso");
        }
    }
    else if (in->tipo == T_LIST)
    {
        lat_imprimir_lista(mv, in->datos.lista);
        fprintf(stdout, "%s\n", "");
    }
    else if (in->tipo == T_FUNC)
    {
        fprintf(stdout, "%s\n", "Funcion");
    }
    else if (in->tipo == T_CFUNC)
    {
        fprintf(stdout, "%s\n", "C_Funcion");
    }
    else if (in->tipo == T_STRUCT)
    {
        fprintf(stdout, "%s\n", "Struct");
    }
    else
    {
        fprintf(stdout, "Tipo desconocido %d\n", in->tipo);
    }
    lat_apilar(mv, in);
}

void lat_imprimir_lista(lat_mv *mv, list_node* l)
{
    fprintf(stdout, "%s", "[ ");
    if (l != NULL && length_list(l) > 0)
    {
        list_node* c;
        for (c = l->next; c != NULL; c = c->next)
        {
            if (c->data != NULL)
            {
                lat_objeto* o = ((lat_objeto*)c->data);
                //printf("\ntype %i, obj_ref: %p\t, marked: %i", o->type, o, o->marked);
                if (o->tipo == T_LIST)
                {
                    lat_imprimir_lista(mv, o->datos.lista);
                    if (c->next->data)
                    {
                        fprintf(stdout, "%s", ", ");
                    }
                }
                else
                {
                    if (o->tipo)
                    {
                        lat_apilar(mv, o);
                        lat_imprimir_elem(mv);
                        if (c->next->data)
                        {
                            fprintf(stdout, "%s", ", ");
                        }
                    }
                }
            }
        }
    }
    fprintf(stdout, "%s", " ]");
}

void lat_imprimir_diccionario(lat_mv *mv, hash_map* d)
{
    fprintf(stdout, "%s", "{ ");
    if (d != NULL)
    {
        list_node* c;
        for (c = (list_node*) d->buckets; c != NULL; c = c->next)
        {
            if (c->data != NULL)
            {
                lat_objeto* o = ((lat_objeto*)c->data);
                //printf("\ntype %i, obj_ref: %p\t, marked: %i", o->type, o, o->marked);
                if (o->tipo == T_LIST)
                {
                    lat_imprimir_lista(mv, o->datos.lista);
                    if (c->next->data)
                    {
                        fprintf(stdout, "%s", ", ");
                    }
                }
                if (o->tipo == T_LIST)
                {
                    lat_imprimir_diccionario(mv, o->datos.diccionario);
                    if (c->next->data)
                    {
                        fprintf(stdout, "%s", ", ");
                    }
                }
                else
                {
                    if (o->tipo)
                    {
                        lat_apilar(mv, o);
                        lat_imprimir_elem(mv);
                        if (c->next->data)
                        {
                            fprintf(stdout, "%s", ", ");
                        }
                    }
                }
            }
        }
    }
    fprintf(stdout, "%s", " ]");
}

void lat_ejecutar(lat_mv *mv)
{
    int status;
    lat_objeto *func = nodo_analizar_arbol(mv, lat_analizar_expresion(mv, lat_obtener_cadena(lat_desapilar(mv)), &status));
    lat_llamar_funcion(mv, func);
    //lat_apilar(mv, mv->registros[255]);
}

void lat_ejecutar_archivo(lat_mv *mv)
{
    char *input = lat_obtener_cadena(lat_desapilar(mv));
    char *dot = strrchr(input, '.');
    char *extension;
    if (!dot || dot == input)
    {
        extension = "";
    }
    else
    {
        extension = dot + 1;
    }
    if (strcmp(extension, "lat") == 0)
    {
        ast *tree = lat_analizar_archivo(mv, input);
        if (!tree)
        {
            lat_registrar_error("error al leer el archivo: %s", input);
        }
        lat_objeto *func = nodo_analizar_arbol(mv, tree);
        lat_llamar_funcion(mv, func);
        //lat_apilar(mv, mv->registros[255]);
    }
}

void lat_clonar(lat_mv *mv)
{
    lat_objeto* ns = lat_desapilar(mv);
    lat_apilar(mv, lat_clonar_objeto(mv, ns));
}

void lat_cons(lat_mv *mv)
{
    lat_objeto* lista = lat_desapilar(mv);
    lat_objeto* elem = lat_desapilar(mv);
    list_node* ret = lat_crear_lista();
    insert_list(ret, (void*)elem);
    ret->next->next = lista->datos.lista;
    lista->datos.lista->prev = ret->next;
    lat_apilar(mv, lat_lista_nueva(mv, ret));
}

void lat_sumar(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if ((a->tipo != T_INT && a->tipo != T_DOUBLE) || (b->tipo != T_INT && b->tipo != T_DOUBLE))
    {
        lat_registrar_error("Intento de aplicar operador \"+\" en tipos invalidos");
    }
    if (a->tipo == T_DOUBLE || b->tipo == T_DOUBLE)
    {
        lat_apilar(mv, lat_decimal_nuevo(mv, lat_obtener_decimal(a) + lat_obtener_decimal(b)));
    }
    else
    {
        lat_apilar(mv, lat_entero_nuevo(mv, lat_obtener_entero(a) + lat_obtener_entero(b)));
    }
}

void lat_restar(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if ((a->tipo != T_INT && a->tipo != T_DOUBLE) || (b->tipo != T_INT && b->tipo != T_DOUBLE))
    {
        lat_registrar_error("Intento de aplicar operador \"-\" en tipos invalidos");
    }
    if (a->tipo == T_DOUBLE || b->tipo == T_DOUBLE)
    {
        lat_apilar(mv, lat_decimal_nuevo(mv, lat_obtener_decimal(a) - lat_obtener_decimal(b)));
    }
    else
    {
        lat_apilar(mv, lat_entero_nuevo(mv, lat_obtener_entero(a) - lat_obtener_entero(b)));
    }
}

void lat_multiplicar(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if ((a->tipo != T_INT && a->tipo != T_DOUBLE) || (b->tipo != T_INT && b->tipo != T_DOUBLE))
    {
        lat_registrar_error("Intento de aplicar operador \"*\" en tipos invalidos");
    }
    if (a->tipo == T_DOUBLE || b->tipo == T_DOUBLE)
    {
        lat_apilar(mv, lat_decimal_nuevo(mv, lat_obtener_decimal(a) * lat_obtener_decimal(b)));
    }
    else
    {
        lat_apilar(mv, lat_entero_nuevo(mv, lat_obtener_entero(a) * lat_obtener_entero(b)));
    }
}

void lat_dividir(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if ((a->tipo != T_INT && a->tipo != T_DOUBLE) || (b->tipo != T_INT && b->tipo != T_DOUBLE))
    {
        lat_registrar_error("Intento de aplicar operador \"/\" en tipos invalidos");
    }
    if ((a->tipo == T_DOUBLE && b->tipo == T_DOUBLE) || (a->tipo == T_INT && b->tipo == T_DOUBLE))
    {
        double tmp = lat_obtener_decimal(b);
        if (tmp == 0)
        {
            lat_registrar_error("Division por cero");
        }
        else
        {
            lat_apilar(mv, lat_decimal_nuevo(mv, (lat_obtener_decimal(a) / tmp)));
        }
    }
    else
    {
        int tmp = lat_obtener_entero(b);
        if (tmp == 0)
        {
            lat_registrar_error("Division por cero");
        }
        else
        {
            if (a->tipo == T_DOUBLE)
            {
                lat_apilar(mv, lat_decimal_nuevo(mv, (lat_obtener_decimal(a) / tmp)));
            }
            else
            {
                lat_apilar(mv, lat_decimal_nuevo(mv, (lat_obtener_entero(a) / tmp)));
            }
        }
    }
}

void lat_modulo(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if (a->tipo != T_INT || b->tipo != T_INT)
    {
        lat_registrar_error("Intento de aplicar operador \"%%\" en tipos invalidos");
    }
    int tmp = lat_obtener_entero(b);
    if (tmp == 0)
    {
        lat_registrar_error("Modulo por cero");
    }
    else
    {
        lat_apilar(mv, lat_entero_nuevo(mv, (lat_obtener_entero(a) % tmp)));
    }
}

void lat_diferente(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if (a->tipo == T_BOOL && b->tipo == T_BOOL)
    {
        lat_apilar(mv, lat_obtener_logico(a) != lat_obtener_logico(b) ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if ((a->tipo == T_INT || a->tipo == T_DOUBLE) && (b->tipo == T_INT || b->tipo == T_DOUBLE))
    {
        lat_apilar(mv, (lat_obtener_decimal(a) != lat_obtener_decimal(b)) ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if (a->tipo == T_STR && b->tipo == T_STR)
    {
        lat_apilar(mv, strcmp(lat_obtener_cadena(a), lat_obtener_cadena(b)) != 0 ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if (a->tipo == T_LIT && b->tipo == T_LIT)
    {
        lat_apilar(mv, strcmp(lat_obtener_literal(a), lat_obtener_literal(b)) != 0 ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    lat_apilar(mv, mv->objeto_falso);
}

void lat_igualdad(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if (a->tipo == T_BOOL && b->tipo == T_BOOL)
    {
        lat_apilar(mv, lat_obtener_logico(a) == lat_obtener_logico(b) ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if ((a->tipo == T_INT || a->tipo == T_DOUBLE) && (b->tipo == T_INT || b->tipo == T_DOUBLE))
    {
        lat_apilar(mv, (lat_obtener_decimal(a) == lat_obtener_decimal(b)) ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if (a->tipo == T_STR && b->tipo == T_STR)
    {
        lat_apilar(mv, strcmp(lat_obtener_cadena(a), lat_obtener_cadena(b)) == 0 ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if (a->tipo == T_LIT && b->tipo == T_LIT)
    {
        lat_apilar(mv, strcmp(lat_obtener_literal(a), lat_obtener_literal(b)) == 0 ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    lat_apilar(mv, mv->objeto_falso);
}

void lat_menor_que(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if ((a->tipo == T_INT || a->tipo == T_DOUBLE) && (b->tipo == T_INT || b->tipo == T_DOUBLE))
    {
        lat_apilar(mv, (lat_obtener_decimal(a) < lat_obtener_decimal(b)) ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if (a->tipo == T_STR && b->tipo == T_STR)
    {
        lat_apilar(mv, strcmp(lat_obtener_cadena(a), lat_obtener_cadena(b)) < 0 ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if (a->tipo == T_LIT && b->tipo == T_LIT)
    {
        lat_apilar(mv, strcmp(lat_obtener_literal(a), lat_obtener_literal(b)) < 0 ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    lat_registrar_error("Intento de aplicar operador \"<\" en tipos invalidos");
}

void lat_menor_igual(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if ((a->tipo == T_INT || a->tipo == T_DOUBLE) && (b->tipo == T_INT || b->tipo == T_DOUBLE))
    {
        lat_apilar(mv, (lat_obtener_decimal(a) <= lat_obtener_decimal(b)) ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if (a->tipo == T_STR && b->tipo == T_STR)
    {
        lat_apilar(mv, strcmp(lat_obtener_cadena(a), lat_obtener_cadena(b)) <= 0 ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if (a->tipo == T_LIT && b->tipo == T_LIT)
    {
        lat_apilar(mv, strcmp(lat_obtener_literal(a), lat_obtener_literal(b)) <= 0 ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    lat_registrar_error("Intento de aplicar operador \"<=\" en tipos invalidos");
}

void lat_mayor_que(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if ((a->tipo == T_INT || a->tipo == T_DOUBLE) && (b->tipo == T_INT || b->tipo == T_DOUBLE))
    {
        lat_apilar(mv, (lat_obtener_decimal(a) > lat_obtener_decimal(b)) ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if (a->tipo == T_STR && b->tipo == T_STR)
    {
        lat_apilar(mv, strcmp(lat_obtener_cadena(a), lat_obtener_cadena(b)) > 0 ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if (a->tipo == T_LIT && b->tipo == T_LIT)
    {
        lat_apilar(mv, strcmp(lat_obtener_literal(a), lat_obtener_literal(b)) > 0 ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    lat_registrar_error("Intento de aplicar operador \">\" en tipos invalidos");
}

void lat_mayor_igual(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if ((a->tipo == T_INT || a->tipo == T_DOUBLE) && (b->tipo == T_INT || b->tipo == T_DOUBLE))
    {
        lat_apilar(mv, (lat_obtener_decimal(a) >= lat_obtener_decimal(b)) ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if (a->tipo == T_STR && b->tipo == T_STR)
    {
        lat_apilar(mv, strcmp(lat_obtener_cadena(a), lat_obtener_cadena(b)) >= 0 ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    if (a->tipo == T_LIT && b->tipo == T_LIT)
    {
        lat_apilar(mv, strcmp(lat_obtener_literal(a), lat_obtener_literal(b)) >= 0 ? mv->objeto_cierto : mv->objeto_falso);
        return;
    }
    lat_registrar_error("Intento de aplicar operador \">=\" en tipos invalidos");
}

void lat_y(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if ((b->tipo != T_BOOL && b->tipo != T_INT) || (a->tipo != T_BOOL && a->tipo != T_INT))
    {
        lat_registrar_error("Intento de aplicar operador \"y\" en tipos invalidos");
    }
    lat_apilar(mv, (lat_obtener_logico(a) && lat_obtener_logico(b)) == true ? mv->objeto_cierto : mv->objeto_falso);
}

void lat_o(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if ((b->tipo != T_BOOL && b->tipo != T_INT) || (a->tipo != T_BOOL && a->tipo != T_INT))
    {
        lat_registrar_error("Intento de aplicar operador \"y\" en tipos invalidos");
    }
    lat_apilar(mv, (lat_obtener_logico(a) || lat_obtener_logico(b)) == true ? mv->objeto_cierto : mv->objeto_falso);
}

void lat_negacion(lat_mv *mv)
{
    lat_objeto* o = lat_desapilar(mv);
    if (o->tipo != T_BOOL && o->tipo != T_INT)
    {
        lat_registrar_error("Intento de negar tipo invalido");
    }
    lat_apilar(mv, lat_obtener_logico(o) == true ? mv->objeto_falso : mv->objeto_cierto);
}

lat_bytecode lat_bc(lat_ins i, void* a, void* b, void* c)
{
    lat_bytecode ret;
    ret.ins = i;
    ret.a = a;
    ret.b = b;
    ret.c = c;
    return ret;
}

void lista_modificar_elemento(list_node* l, void* data, int pos)
{
    list_node* c;
    int i = -1;
    if(pos < 0 || pos >= length_list(l))
    {
        lat_registrar_error("Indice fuera de rango");
    }
    for (c = l; c->next != NULL; c = c->next)
    {
        if(i == pos)
        {
            c->data = data;
            return;
        }
        i++;
    }
}

lat_objeto* lista_obtener_elemento(list_node* l, int pos)
{
    list_node* c;
    int i = -1;
    if(pos < 0 || pos >= length_list(l))
    {
        lat_registrar_error("Indice fuera de rango");
    }
    for (c = l; c->next != NULL; c = c->next)
    {
        if(i == pos)
        {
            return (lat_objeto *)c->data;
        }
        i++;
    }
    return NULL;
}

lat_objeto* lat_llamar_funcion(lat_mv *mv, lat_objeto* func)
{
    if (func->tipo == T_FUNC)
    {
        if(!mv->REPL)
        {
            lat_apilar_contexto(mv);
        }
        lat_asignar_contexto_objeto(lat_obtener_contexto(mv), lat_cadena_nueva(mv, "$"), func);
        lat_bytecode* inslist = ((lat_funcion*)func->datos.funcion)->bcode;
        lat_bytecode cur;
        int pos;
        for (pos = 0, cur = inslist[pos]; cur.ins != RETURN_VALUE; cur = inslist[++pos])
        {
            switch (cur.ins)
            {
            /*case OP_NOP:
                break;
            case OP_PUSH:
                lat_apilar(mv, mv->registros[cur.a]);
                break;
            case OP_POP:
                mv->registros[cur.a] = lat_desapilar(mv);
                break;
            case OP_GET:
                mv->registros[cur.a] = lat_obtener_contexto_objeto(mv->registros[cur.b], mv->registros[cur.a]);
                break;
            case OP_SET:
                lat_asignar_contexto_objeto(mv->registros[cur.b], lat_clonar_objeto(mv, ((lat_objeto*)cur.meta)), mv->registros[cur.a]);
                break;
            case OP_STORELIT:
                mv->registros[cur.a] = ((lat_objeto*)cur.meta);
                break;
            case OP_STOREINT:
                mv->registros[cur.a] = ((lat_objeto*)cur.meta);
                break;
            case OP_STOREDOUBLE:
                mv->registros[cur.a] = ((lat_objeto*)cur.meta);
                break;
            case OP_STORESTR:
            {
                mv->registros[cur.a] = ((lat_objeto*)cur.meta);
            }
            break;
            case OP_STOREBOOL:
                mv->registros[cur.a] = ((lat_objeto*)cur.meta);
                break;

            case OP_STORELIST:
                mv->registros[cur.a] = lat_lista_nueva(mv, lat_crear_lista());
                break;

            case OP_PUSHLIST:
                lat_apilar_lista(mv->registros[cur.a], mv->registros[cur.b]);
                break;
            case OP_POPLIST:
                //TODO: Pendiente
                //mv->registros[cur.a] = lat_desapilar_lista(mv->registros[cur.b]);
                break;
            case OP_LISTGETITEM:
            {
                lat_objeto *l = mv->registros[cur.a];
                lat_objeto *pos = mv->registros[cur.b];
                mv->registros[cur.a] = lista_obtener_elemento(l->datos.lista, pos->datos.entero);
            }
            break;
            case OP_LISTSETITEM:
            {
                lat_objeto *l = mv->registros[cur.a];
                lat_objeto *pos = mv->registros[(int)cur.meta];
                if(pos->tipo != T_INT)
                {
                    lat_registrar_error("%s", "la posicion de la lista no es un entero");
                }
                lista_modificar_elemento(l->datos.lista, (lat_objeto*)mv->registros[cur.b], pos->datos.entero);
            }
            break;
            case OP_STOREDICT:
                //TODO: Pendiente
                //mv->registros[cur.a] = lat_lista_nueva(mv, make_dict());
                break;
            case OP_PUSHDICT:
                //TODO: Pendiente
                //lat_push_dict(mv->registros[cur.a], mv->registros[cur.b]);
                break;
            case OP_POPDICT:
                //TODO: Pendiente
                //mv->registros[cur.a] = lat_pop_dict(mv->registros[cur.b]);
                break;
            case OP_MOV:
                mv->registros[cur.a] = mv->registros[cur.b];
                break;
            case OP_GLOBALNS:
                mv->registros[cur.a] = mv->contexto_pila[0];
                break;
            case OP_LOCALNS:
                mv->registros[cur.a] = lat_obtener_contexto(mv);
                break;
            case OP_NS:
                mv->registros[cur.a] = lat_clonar_objeto(mv, lat_obtener_contexto(mv));
                lat_apilar_contexto_predefinido(mv, mv->registros[cur.a]);
                break;
            case RETURN_VALUENS:
                mv->registros[cur.a] = lat_desapilar_contexto_predefinido(mv);
                break;
            case OP_JMP:
                pos = cur.a - 1;
                break;
            case OP_JMPIF:
                if (lat_obtener_logico(mv->registros[cur.b]))
                {
                    pos = cur.a - 1;
                }
                break;
            case OP_NOT:
                mv->registros[cur.a] = lat_obtener_logico(mv->registros[cur.a]) == true ? mv->objeto_falso : mv->objeto_cierto;
                break;
            case OP_INC:
                ((lat_objeto*)mv->registros[cur.a])->datos.entero++;
                break;
            case OP_DEC:
                ((lat_objeto*)mv->registros[cur.a])->datos.entero--;
                break;
            */

            /* redefinicion de instrucciones estilo Python*/
            case LOAD_CONST:
                lat_apilar(mv, (lat_objeto*)cur.a);
                break;
            case STORE_NAME:{
                    lat_objeto *contexto = lat_obtener_contexto(mv);
                    lat_objeto *variable = (lat_objeto*)cur.a;
                    lat_objeto *valor = lat_desapilar(mv);
                    lat_asignar_contexto_objeto(contexto, variable, valor);                    
                }
                break;
            case LOAD_NAME: {
                    lat_objeto *contexto = lat_obtener_contexto(mv);
                    lat_objeto *variable = (lat_objeto*)cur.a;
                    lat_objeto *valor = lat_obtener_contexto_objeto(contexto, variable);
                    lat_apilar(mv, valor);
                }
                break;
            case MAKE_FUNCTION: {                    
                    lat_objeto* funcion_usuario = lat_definir_funcion(mv, (lat_bytecode*)cur.a, (int*)cur.b);                    
                    lat_apilar(mv, funcion_usuario);
                }
                break;
            case CALL_FUNCTION:
                {
                    lat_objeto* funcion = lat_desapilar(mv);
                    lat_objeto* resultado = NULL;
                    if(funcion->tipo == T_FUNC){
                        resultado = lat_llamar_funcion(mv, funcion);
                        if(resultado == NULL) {
                            resultado = lat_desapilar(mv);
                        }
                    }else{
                        lat_llamar_funcion(mv, funcion);
                        //resultado = lat_desapilar(mv);                        
                    }                    
                }
                break;
            case RETURN_VALUE:
                {
                    lat_objeto* res = lat_desapilar(mv);
                    return res;
                }
                break;
            case BINARY_ADD:
                lat_sumar(mv);                
                break;
            case BINARY_SUBTRACT:
                lat_restar(mv);                
                break;
            case BINARY_MULTIPLY:
                lat_multiplicar(mv);                
                break;
            case BINARY_FLOOR_DIVIDE:
                lat_dividir(mv);                
                break;
            case BINARY_MODULO:
                lat_modulo(mv);                
                break;
                
            }   //end switch
        }   //end for
        if(!mv->REPL)
        {
            lat_desapilar_contexto(mv);
        }
    }
    else if (func->tipo == T_CFUNC)
    {
        ((void (*)(lat_mv*))(func->datos.funcion))(mv);
    }
    else
    {
        debug("func->type: %d", func->tipo);
        lat_registrar_error("Object not a function");
    }
}


void lat_logico(lat_mv *mv)
{
    lat_objeto* a = lat_desapilar(mv);
    switch (a->tipo)
    {
    case T_INT:
        if(a->datos.entero == 0)
        {
            lat_apilar(mv, mv->objeto_falso);
        }
        else
        {
            lat_apilar(mv, mv->objeto_cierto);
        }
        break;
    case T_LIT:
        if(strcmp(a->datos.literal, "") == 0)
        {
            lat_apilar(mv, mv->objeto_falso);
        }
        else
        {
            lat_apilar(mv, mv->objeto_cierto);
        }
        break;
    case T_DOUBLE:
        if((int)a->datos.decimal == 0)
        {
            lat_apilar(mv, mv->objeto_falso);
        }
        else
        {
            lat_apilar(mv, mv->objeto_cierto);
        }
        break;
    case T_STR:
        if(strcmp(a->datos.cadena, "") == 0)
        {
            lat_apilar(mv, mv->objeto_falso);
        }
        else
        {
            lat_apilar(mv, mv->objeto_cierto);
        }
        break;
    default:
        lat_registrar_error("conversion incompatible");
        break;
    }
}
void lat_entero(lat_mv *mv)
{
    lat_objeto* a = lat_desapilar(mv);
    switch (a->tipo)
    {
    case T_BOOL:
        if(a->datos.logico == false)
        {
            lat_apilar(mv, lat_entero_nuevo(mv, 0));
        }
        else
        {
            lat_apilar(mv, lat_entero_nuevo(mv, 1));
        }
        break;
    case T_LIT:
    {
        char *ptr;
        long ret;
        ret =strtol(a->datos.literal, &ptr, 10);
        if(strcmp(ptr, "") == 0)
        {
            lat_apilar(mv, lat_entero_nuevo(mv, ret));
        }
        else
        {
            lat_registrar_error("conversion incompatible");
        }
    }
    break;
    case T_DOUBLE:
        lat_apilar(mv, lat_entero_nuevo(mv, (int)a->datos.decimal));
        break;
    case T_STR:
    {
        char *ptr;
        long ret;
        ret =strtol(a->datos.cadena, &ptr, 10);
        if(strcmp(ptr, "") == 0)
        {
            lat_apilar(mv, lat_entero_nuevo(mv, ret));
        }
        else
        {
            lat_registrar_error("conversion incompatible");
        }
    }
    break;
    default:
        lat_registrar_error("conversion incompatible");
        break;
    }
}
void lat_literal(lat_mv *mv)
{
    lat_objeto* a = lat_desapilar(mv);
    switch (a->tipo)
    {
    case T_BOOL:
        if(a->datos.logico == false)
        {
            lat_apilar(mv, lat_literal_nuevo(mv, "falso"));
        }
        else
        {
            lat_apilar(mv, lat_literal_nuevo(mv, "verdadero"));
        }
        break;
    case T_STR:
    {
        lat_apilar(mv, a);
    }
    break;
    default:
        lat_registrar_error("conversion incompatible");
        break;
    }
}
void lat_decimal(lat_mv *mv)
{
    lat_objeto* a = lat_desapilar(mv);
    switch (a->tipo)
    {
    case T_BOOL:
        if(a->datos.logico == false)
        {
            lat_apilar(mv, lat_decimal_nuevo(mv, 0));
        }
        else
        {
            lat_apilar(mv, lat_decimal_nuevo(mv, 1));
        }
        break;
    case T_INT:
        lat_apilar(mv, lat_decimal_nuevo(mv, (double)a->datos.entero));
        break;
    case T_DOUBLE:
        lat_apilar(mv, lat_decimal_nuevo(mv, (double)a->datos.entero));
        break;
    case T_STR:
    {
        char *ptr;
        double ret;
        ret =strtod(a->datos.cadena, &ptr);
        if(strcmp(ptr, "") == 0)
        {
            lat_apilar(mv, lat_decimal_nuevo(mv, ret));
        }
        else
        {
            lat_registrar_error("conversion incompatible");
        }
    }
    break;
    default:
        lat_registrar_error("conversion incompatible");
        break;
    }
}

void lat_cadena(lat_mv *mv)
{
    lat_objeto* a = lat_desapilar(mv);
    switch (a->tipo)
    {
    case T_BOOL:
        lat_apilar(mv, lat_cadena_nueva(mv, bool2str(a->datos.logico)));
        break;
    case T_INT:
        lat_apilar(mv, lat_cadena_nueva(mv, int2str(a->datos.entero)));
        break;
    case T_DOUBLE:
        lat_apilar(mv, lat_cadena_nueva(mv, double2str(a->datos.decimal)));
        break;
    default:
        lat_apilar(mv, a);
        break;
    }
}

void lat_maximo(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if(lat_obtener_entero(b) > lat_obtener_entero(a))
    {
        lat_apilar(mv, b);
    }
    else
    {
        lat_apilar(mv, a);
    }
}

void lat_minimo(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if(lat_obtener_entero(b) < lat_obtener_entero(a))
    {
        lat_apilar(mv, b);
    }
    else
    {
        lat_apilar(mv, a);
    }
}

void lat_tipo(lat_mv *mv)
{
    lat_objeto* a = lat_desapilar(mv);
    switch (a->tipo)
    {
    case T_BOOL:
        lat_apilar(mv, lat_cadena_nueva(mv, "logico"));
        break;
    case T_INT:
        lat_apilar(mv, lat_cadena_nueva(mv, "entero"));
        break;
    case T_DOUBLE:
        lat_apilar(mv, lat_cadena_nueva(mv, "decimal"));
        break;
    case T_STR:
        lat_apilar(mv, lat_cadena_nueva(mv, "cadena"));
        break;
    case T_LIT:
        lat_apilar(mv, lat_cadena_nueva(mv, "cadena"));
        break;
    case T_LIST:
        lat_apilar(mv, lat_cadena_nueva(mv, "lista"));
        break;
    case T_DICT:
        lat_apilar(mv, lat_cadena_nueva(mv, "diccionario"));
        break;
    default:
        lat_apilar(mv, lat_cadena_nueva(mv, "nulo"));
        break;
    }
}

void lat_salir(lat_mv *mv)
{
    lat_apilar(mv, lat_entero_nuevo(mv, 0L));
    exit(0);
}
