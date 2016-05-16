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

#include <math.h>
#include "libmath.h"
#include "vm.h"

void lat_arco_coseno(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, acos(lat_obtener_decimal(a)));
}

void lat_arco_seno(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, asin(lat_obtener_decimal(a)));
}

void lat_arco_tangente(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, atan(lat_obtener_decimal(a)));
}

void lat_arco_tangente2(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    lat_objeto *b = lat_desapilar(mv);
    mv->registros[255] =
        lat_decimal_nuevo(mv, atan2(lat_obtener_decimal(a), lat_obtener_decimal(b)));
}

void lat_coseno(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, cos(lat_obtener_decimal(a)));
}

void lat_coseno_hiperbolico(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, cosh(lat_obtener_decimal(a)));
}

void lat_seno(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, sin(lat_obtener_decimal(a)));
}

void lat_seno_hiperbolico(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, sinh(lat_obtener_decimal(a)));
}

void lat_tangente(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, tan(lat_obtener_decimal(a)));
}

void lat_tangente_hiperbolica(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, tanh(lat_obtener_decimal(a)));
}

void lat_exponente(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, exp(lat_obtener_decimal(a)));
}

/*
void lat_frexp(lat_mv *mv){
  lat_objeto *a = lat_desapilar(mv);
  mv->registros[255] = lat_decimal_nuevo(mv, frexp(lat_obtener_decimal(a)));
}
*/

/*
void lat_ldexp(lat_mv *mv)
{
  lat_objeto* a = lat_desapilar(mv);
  lat_objeto* b = lat_desapilar(mv);
  mv->registros[255] = lat_decimal_nuevo(mv, ldexp(lat_obtener_decimal(a),
lat_obtener_entero(b)));
}
*/

void lat_logaritmo_natural(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, log(lat_obtener_decimal(a)));
}

void lat_logaritmo_base10(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, log10(lat_obtener_decimal(a)));
}

/*
void lat_modf(lat_mv *mv){
  lat_objeto *a = lat_desapilar(mv);
  mv->registros[255] = lat_decimal_nuevo(mv, modf(lat_obtener_decimal(a)));
}
*/

void lat_potencia(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    lat_objeto *b = lat_desapilar(mv);
    mv->registros[255] =
        lat_decimal_nuevo(mv, pow(lat_obtener_decimal(a), lat_obtener_decimal(b)));
}

void lat_raiz_cuadrada(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, sqrt(lat_obtener_decimal(a)));
}

void lat_redondear_arriba(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, ceil(lat_obtener_decimal(a)));
}

void lat_valor_absoluto(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, fabs(lat_obtener_decimal(a)));
}

void lat_redondeo_abajo(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    mv->registros[255] = lat_decimal_nuevo(mv, floor(lat_obtener_decimal(a)));
}

void lat_modulo_decimal(lat_mv *mv)
{
    lat_objeto *a = lat_desapilar(mv);
    lat_objeto *b = lat_desapilar(mv);
    mv->registros[255] =
        lat_decimal_nuevo(mv, fmod(lat_obtener_decimal(a), lat_obtener_decimal(b)));
}
