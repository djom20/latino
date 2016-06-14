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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "linenoise/linenoise.h"
#include "latino.h"
#include "parse.h"
#include "lex.h"
#include "ast.h"

/* 1 para debuguear analizador */
int yydebug = 0;
int analisis_silencioso;
static FILE *file;
static char *buffer;

int yyparse(ast **root, yyscan_t scanner);

int lat_analizar_expresion(lat_mv *mv, ast** nodo, char* expr)
{
    setlocale (LC_ALL, "");
    yyscan_t scanner;
    YY_BUFFER_STATE state;
    lex_state scan_state = {.insert = 0};
    yylex_init_extra(&scan_state, &scanner);
    state = yy_scan_string(expr, scanner);
    int status = yyparse(nodo, scanner);
    yy_delete_buffer(state, scanner);
    yylex_destroy(scanner);
    return status;
}

int lat_analizar_archivo(lat_mv *mv, ast** nodo, char *infile)
{
    if (infile == NULL)
    {
        printf("Especifique un archivo\n");
        return 1;
    }
    char *dot = strrchr(infile, '.');
    char *extension;
    if (!dot || dot == infile)
        extension = "";
    else
        extension = dot + 1;
    if (strcmp(extension, "lat") != 0)
    {
        printf("El archivo no contiene la extension .lat\n");
        return 1;
    }
    file = fopen(infile, "r");
    if (file == NULL)
    {
        printf("No se pudo abrir el archivo\n");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    int fsize = ftell(file);
    fseek(file, 0, SEEK_SET);    
    buffer = malloc(fsize+1);   //+1 para fin de cadena '\0'
    size_t newSize = fread(buffer, sizeof(char), fsize, file);
    if (buffer == NULL)
    {
        printf("No se pudo asignar %i bytes de memoria\n", fsize);
        return 1;
    }
    buffer[newSize] = '\0';
    int status = lat_analizar_expresion(mv, nodo, buffer);
    return status;
}
/**
 * Muestra la version de latino en la consola
 */
static void lat_version()
{
    printf("%s\n", LAT_DERECHOS);
}
/**
 * Para crear el logo en ascii
 */
static void lat_logo()
{
    printf("%s\n", LAT_LOGO);
}

/**
 * Muestra la ayuda en la consola
 */
static void lat_ayuda()
{
    lat_logo();
    lat_version();
    printf("%s\n", "Uso de Latino: latino [opcion] [archivo]");
    printf("\n");
    printf("%s\n", "Opciones:");
    printf("%s\n", "-a               : Muestra la ayuda de Latino");
    printf("%s\n", "-i               : Inicia el interprete de Latino (Modo interactivo)");
    printf("%s\n", "-v               : Muestra la version de Latino");
    printf("%s\n", "archivo          : Nombre del archivo con extension .lat");
    printf("%s\n", "Ctrl-C o salir() : Para cerrar interprete");
    printf("\n");
    printf("%s\n", "Variables de entorno:");
    printf("%s\n", "_____________________");
    printf("%s%s\n", "LATINO_PATH  : ", getenv("LATINO_PATH"));
    printf("%s%s\n", "LATINO_LIB   : ", getenv("LATINO_LIB"));
    printf("%s%s\n", "LC_LANG      : ", getenv("LC_LANG"));
    printf("%s%s\n", "HOME         : ", getenv("HOME"));
}

static int leer_linea(lat_mv *mv, char* buffer)
{
    analisis_silencioso = 1;
    int resultado = -1;
    char *input = "";    
    char *tmp = "";
REPETIR:
    input = linenoise("latino> ");
    if(input == NULL)
    {
        return -1;
    }
    for(;;)
    {
        tmp = concat(tmp, "\n");
        tmp = concat(tmp, input);
        int estatus;
        ast* nodo = malloc(sizeof(ast));
        estatus = lat_analizar_expresion(mv, &nodo, tmp);
        if(estatus == 1)
        {
            goto REPETIR;
        }
        else
        {
            strcpy(buffer, tmp);
            return 0;
        }
    }
    return resultado;
}

/*
 * Agrega autocompletado para REPL (Read Eval Print Loop)
 */
static void completion(const char *buf, linenoiseCompletions *lc)
{
    if (startsWith(buf, "esc"))
        linenoiseAddCompletion(lc,"escribir");
    if (startsWith(buf, "imp"))
        linenoiseAddCompletion(lc,"imprimir");
    if (startsWith(buf, "eje"))
        linenoiseAddCompletion(lc,"ejecutar");
    if (startsWith(buf, "ejea"))
        linenoiseAddCompletion(lc,"ejecutar_archivo");
    if (startsWith(buf, "fun"))
        linenoiseAddCompletion(lc,"funcion");
    if (startsWith(buf, "com"))
        linenoiseAddCompletion(lc,"comparar");
    if (startsWith(buf, "con"))
        linenoiseAddCompletion(lc,"concatenar");
    if (startsWith(buf, "cont"))
        linenoiseAddCompletion(lc,"contiene");
    if (startsWith(buf, "cop"))
        linenoiseAddCompletion(lc,"copiar");
    if (startsWith(buf, "ter"))
        linenoiseAddCompletion(lc,"termina_con");
    if (startsWith(buf, "es_"))
        linenoiseAddCompletion(lc,"es_igual");
    if (startsWith(buf, "ind"))
        linenoiseAddCompletion(lc,"indice");
    if (startsWith(buf, "ins"))
        linenoiseAddCompletion(lc,"insertar");
    if (startsWith(buf, "ult"))
        linenoiseAddCompletion(lc,"ultimo_indice");
    if (startsWith(buf, "reli"))
        linenoiseAddCompletion(lc,"rellenar_izquierda");
    if (startsWith(buf, "reld"))
        linenoiseAddCompletion(lc,"rellenar_derecha");
    if (startsWith(buf, "eli"))
        linenoiseAddCompletion(lc,"eliminar");
    if (startsWith(buf, "est"))
        linenoiseAddCompletion(lc,"esta_vacia");
    if (startsWith(buf, "lon"))
        linenoiseAddCompletion(lc,"longitud");
    if (startsWith(buf, "ree"))
        linenoiseAddCompletion(lc,"reemplazar");
    if (startsWith(buf, "emp"))
        linenoiseAddCompletion(lc,"empieza_con");
    if (startsWith(buf, "sub"))
        linenoiseAddCompletion(lc,"subcadena");
    if (startsWith(buf, "min"))
        linenoiseAddCompletion(lc,"minusculas");
    if (startsWith(buf, "may"))
        linenoiseAddCompletion(lc,"mayusculas");
    if (startsWith(buf, "qui"))
        linenoiseAddCompletion(lc,"quitar_espacios");
    if (startsWith(buf, "lee"))
        linenoiseAddCompletion(lc,"leer");
    if (startsWith(buf, "esca"))
        linenoiseAddCompletion(lc,"escribir_archivo");
    if (startsWith(buf, "tip"))
        linenoiseAddCompletion(lc,"tipo");
    if (startsWith(buf, "log"))
        linenoiseAddCompletion(lc,"logico");
    if (startsWith(buf, "ent"))
        linenoiseAddCompletion(lc,"entero");
    if (startsWith(buf, "dec"))
        linenoiseAddCompletion(lc,"decimal");
    if (startsWith(buf, "cad"))
        linenoiseAddCompletion(lc,"cadena");
    if (startsWith(buf, "sal"))
        linenoiseAddCompletion(lc,"salir");
}

static void lat_REPL(lat_mv *mv)
{
    ast* nodo = malloc(sizeof(ast));
    char* buf = malloc(MAX_STR_INTERN*sizeof(char));
    mv->REPL = true;
    linenoiseHistoryLoad("history.txt");
    linenoiseSetCompletionCallback(completion);
    while (leer_linea(mv, buf) != -1)
    {
        analisis_silencioso = 0;
        int status = lat_analizar_expresion(mv, &nodo, buf);
        if(status == 0)
        {
            lat_objeto* curexpr = nodo_analizar_arbol(mv, nodo);
            lat_llamar_funcion(mv, curexpr);
            /*lat_objeto* resultado = lat_llamar_funcion(mv, curexpr);
            if(resultado->tipo && !contains(buf, "escribir") && !contains(buf, "imprimir"))
            {
                lat_apilar(mv, resultado);
                lat_imprimir(mv);
            }*/
            linenoiseHistoryAdd(replace(buf, "\n", ""));
            linenoiseHistorySave("history.txt");
        }
    }
    free(buf);
    free(nodo);
}

int main(int argc, char *argv[])
{
    /*
    Para debuguear en visual studio:
    Menu propiedades del proyecto-> Debugging -> Command Arguments. Agregar
    $(SolutionDir)..\ejemplos\debug.lat
    */

    int i;
    char *infile = NULL;
    lat_mv *mv = lat_crear_maquina_virtual();
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0)
        {
            lat_version();
            return EXIT_SUCCESS;
        }
        else if (strcmp(argv[i], "-a") == 0)
        {
            lat_ayuda();
            return EXIT_SUCCESS;
        }
        else if (strcmp(argv[i], "-i") == 0)
        {
            lat_version();
            lat_REPL(mv);
            return EXIT_SUCCESS;
        }
        else
        {
            infile = argv[i];
        }
    }
    if(argc > 1 && infile != NULL)
    {
        mv->REPL = false;
        ast *nodo = malloc(sizeof(ast));
        int estado = lat_analizar_archivo(mv, &nodo, infile);
        if (estado)
        {
            return EXIT_FAILURE;
        }
        lat_objeto* mainFunc = nodo_analizar_arbol(mv, nodo);
        //printf("---------------------------------------------\n");
        lat_llamar_funcion(mv, mainFunc);
        if(file != NULL)
        {
            fclose(file);
        }
        free(nodo);
    }
    else
    {
#ifdef _WIN32
        //system("cmd");
        lat_version();
        lat_REPL(mv);
#else
        lat_version();
        lat_REPL(mv);
#endif
    }

    lat_eliminar_maquina_virtual(mv);
    return EXIT_SUCCESS;
}
