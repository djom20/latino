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

#include "khash.h"
#include "latino.h"
#include "object.h"
#include "libmem.h"
#include "utils.h"

KHASH_MAP_INIT_INT64(env, lat_objeto);
typedef khash_t(env) lat_env;
lat_env* globals;

struct sym_key
{
    const char* ptr;
    size_t len;
};

static khint_t
sym_hash(struct sym_key key)
{
    const char* s = key.ptr;
    khint_t h;
    size_t len = key.len;
    h = *s++;
    while (len--)
    {
        h = (h << 5) - h + (khint_t)*s++;
    }
    return h;
}

static khint_t
sym_eq(struct sym_key a, struct sym_key b)
{
    if (a.len != b.len)
        return false;
    if (memcmp(a.ptr, b.ptr, a.len) == 0)
        return true;
    return false;
}

KHASH_INIT(sym, struct sym_key, lat_objeto*, 1, sym_hash, sym_eq);
static khash_t(sym) * sym_table;

static lat_objeto* str_new(const char* p, size_t len)
{
    lat_objeto* str = (lat_objeto*)lat_asignar_memoria(sizeof(lat_objeto));
    str->tipo = T_STR;
    str->tamanio_datos = len;
    str->datos.cadena = (char *)p;
    return str;
}

static lat_objeto* str_intern(const char* p, size_t len)
{
    khiter_t k;
    struct sym_key key;
    int ret;
    lat_objeto* str;
    if (!sym_table)
    {
        sym_table = kh_init(sym);
    }
    key.ptr = p;
    key.len = len;
    k = kh_put(sym, sym_table, key, &ret);
    if (ret == 0)
    {
        return kh_value(sym_table, k);
    }
    str = str_new(p, len);
    kh_key(sym_table, k).ptr = str->datos.cadena;
    kh_value(sym_table, k) = str;
    return str;
}

lat_objeto* lat_cadena_hash(const char* p, size_t len)
{
    if (p && (len < MAX_STR_INTERN))
    {
        return str_intern(p, len);
    }
    return str_new(p, len);
}

/*
void lat_comparar(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    lat_apilar(mv, lat_entero_nuevo(mv, strcmp(lat_obtener_cadena(a), lat_obtener_cadena(b))));
}

void lat_concatenar(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    lat_objeto* x = NULL;
    lat_objeto* y = NULL;
    switch (a->tipo)
    {
    case T_BOOL:
        x = lat_cadena_nueva(mv, bool2str(a->datos.logico));
        break;
    case T_INT:
        x = lat_cadena_nueva(mv, int2str(a->datos.entero));
        break;
    case T_DOUBLE:
        x = lat_cadena_nueva(mv, double2str(a->datos.decimal));
        break;
    default:
        x = lat_cadena_nueva(mv, a->datos.cadena);
        break;
    }

    switch (b->tipo)
    {
    case T_BOOL:
        y = lat_cadena_nueva(mv, bool2str(b->datos.logico));
        break;
    case T_INT:
        y = lat_cadena_nueva(mv, int2str(b->datos.entero));
        break;
    case T_DOUBLE:
        y = lat_cadena_nueva(mv, double2str(b->datos.decimal));
        break;
    default:
        y = lat_cadena_nueva(mv, b->datos.cadena);
        break;
    }
    lat_apilar(mv, lat_cadena_nueva(mv, concat(lat_obtener_cadena(x), lat_obtener_cadena(y))));
}

void lat_contiene(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    char *result = strstr(lat_obtener_cadena(a), lat_obtener_cadena(b));
    if (result != NULL)
    {
        lat_apilar(mv, mv->objeto_cierto);
    }
    else
    {
        lat_apilar(mv, mv->objeto_falso);
    }
}

void lat_copiar(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_apilar(mv, lat_clonar_objeto(mv, b));
}

void lat_termina_con(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if (endsWith(lat_obtener_cadena(a), lat_obtener_cadena(b)))
    {
        lat_apilar(mv, mv->objeto_cierto);
    }
    else
    {
        lat_apilar(mv, mv->objeto_falso);
    }
}

void lat_es_igual(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if (strcmp(lat_obtener_cadena(a), lat_obtener_cadena(b)) == 0)
    {
        lat_apilar(mv, mv->objeto_cierto);
    }
    else
    {
        lat_apilar(mv, mv->objeto_falso);
    }
}

void lat_indice(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    lat_apilar(mv, lat_entero_nuevo(mv, indexOf(lat_obtener_cadena(a), lat_obtener_cadena(b))));
}

void lat_insertar(lat_mv *mv)
{
    lat_objeto* c = lat_desapilar(mv);
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    lat_apilar(mv, lat_cadena_nueva(mv, insert(lat_obtener_cadena(a), lat_obtener_cadena(b), lat_obtener_entero(c))));
}

void lat_ultimo_indice(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    lat_apilar(mv, lat_entero_nuevo(mv, lastIndexOf(lat_obtener_cadena(a), lat_obtener_cadena(b))));
}

void lat_rellenar_izquierda(lat_mv *mv)
{
    lat_objeto* c = lat_desapilar(mv);
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    lat_apilar(mv, lat_cadena_nueva(mv, padLeft(lat_obtener_cadena(a), lat_obtener_entero(b), lat_obtener_literal(c))));
}

void lat_rellenar_derecha(lat_mv *mv)
{
    lat_objeto* c = lat_desapilar(mv);
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    lat_apilar(mv, lat_cadena_nueva(mv, padRight(lat_obtener_cadena(a), lat_obtener_entero(b), lat_obtener_literal(c))));
}

void lat_eliminar(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    lat_apilar(mv, lat_cadena_nueva(mv, replace(lat_obtener_cadena(a), lat_obtener_cadena(b), "")));
}

void lat_esta_vacia(lat_mv *mv)
{
    lat_objeto* a = lat_desapilar(mv);
    if (strcmp(lat_obtener_cadena(a), "") == 0)
    {
        lat_apilar(mv, mv->objeto_cierto);
    }
    else
    {
        lat_apilar(mv, mv->objeto_falso);
    }
}

void lat_longitud(lat_mv *mv)
{
    lat_objeto* a = lat_desapilar(mv);
    lat_apilar(mv, lat_entero_nuevo(mv, strlen(lat_obtener_cadena(a))));
}

void lat_reemplazar(lat_mv *mv)
{
    lat_objeto* c = lat_desapilar(mv);
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    lat_apilar(mv, lat_cadena_nueva(mv, replace(lat_obtener_cadena(a), lat_obtener_cadena(b), lat_obtener_cadena(c))));
}

void lat_empieza_con(lat_mv *mv)
{
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    if (startsWith(lat_obtener_cadena(a), lat_obtener_cadena(b)))
    {
        lat_apilar(mv, mv->objeto_cierto);
    }
    else
    {
        lat_apilar(mv, mv->objeto_falso);
    }
}

void lat_subcadena(lat_mv *mv)
{
    lat_objeto* c = lat_desapilar(mv);
    lat_objeto* b = lat_desapilar(mv);
    lat_objeto* a = lat_desapilar(mv);
    lat_apilar(mv, lat_cadena_nueva(mv, substring(lat_obtener_cadena(a), lat_obtener_entero(b), lat_obtener_entero(c))));
}

void lat_minusculas(lat_mv *mv)
{
    lat_objeto* a = lat_desapilar(mv);
    lat_apilar(mv, lat_cadena_nueva(mv, toLower(lat_obtener_cadena(a))));
}

void lat_mayusculas(lat_mv *mv)
{
    lat_objeto* a = lat_desapilar(mv);
    lat_apilar(mv, lat_cadena_nueva(mv, toUpper(lat_obtener_cadena(a))));
}

void lat_quitar_espacios(lat_mv *mv)
{
    lat_objeto* a = lat_desapilar(mv);
    lat_apilar(mv, lat_cadena_nueva(mv, trim(lat_obtener_cadena(a))));
}
*/