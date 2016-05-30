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
#ifndef _VM_H_
#define _VM_H_

/** \file vm.h
  *
  * Contiene funciones para el manejo de la maquina virtual
  *
  */

/**\brief Objeto tipo maquina virtual */
typedef struct lat_mv lat_mv;

#include "utils.h"
#include "object.h"

/**\brief Bandera para debuguear las instrucciones de la maquina virtual */
#define DEPURAR_MV 1

/**\brief Instrucciones de la maquina virtual */
typedef enum lat_ins
{    
    OP_PUSH,    /**< Inserta en la pila */
    OP_POP,    /**< Extrae de la pila */
    OP_GET,    /**< Obtiene un valor */
    OP_SET,    /**< Establece un valor */
    //OP_STORELIT,    /**< Almacena un cadena tipo literal */
    //OP_STOREINT,    /**< Almacena un entero */
    //OP_STOREDOUBLE,    /**< Almacena un decimal */
    //OP_STORESTR,    /**< Almacena una cadena */
    //OP_STOREBOOL,    /**< Almacena un valor logico */
    //OP_STORELIST,    /**< Almacena una lista */
    OP_PUSHLIST,    /**< Inserta la lista en la pila */
    OP_POPLIST,    /**< Extrae la lista de la pila */
    OP_LISTGETITEM,    /**< Obtiene un valor de la lista */
    OP_LISTSETITEM,    /**< Asigna un valor de la lista */
    OP_MOV,    /**< Mueve valores a los registros de la MV */
    OP_GLOBALNS,    /**< Almacena variables globales */
    OP_LOCALNS,    /**< Almacena variables locales */
    OP_NS,    /**< Define un contexto */
    RETURN_VALUENS,    /**< Define el fin de un contexto */
    OP_JMP,    /**< Saltar a otra instruccion */
    OP_JMPIF,    /**< Saltar a otra instruccion SI la condicion se cumple */
    OP_NOT,    /**< Negacion de un valor logico */
    OP_INC,    /**< Incrementa un valor entero en 1 */
    OP_DEC,    /**< Disminuye un valor entero en 1 */
    OP_STOREDICT,    /**< Almacena un objeto diccionario */
    OP_PUSHDICT,    /**< Inserta un objeto diccionario en la pila */
    OP_POPDICT,    /**< Extrae un objeto diccionario de la pila */
    OP_PUSHDICTELEM, /**< Crea un objeto par (llave-valor) y lo agrega al diccionario de la pila */
    OP_DICTGETITEM,    /**< Obtiene un valor del diccionario */
    OP_DICTSETITEM,    /**< Asigna un valor del diccionario */
    
    /* redefinicion de instrucciones */
    NOP,    /**< Indica No operacion */
    LOAD_CONST,
    STORE_NAME,
    LOAD_NAME,
    CALL_FUNCTION,          /**< Llamada a una funcion */
    MAKE_FUNCTION,          /**< Define una funcion */
    RETURN_VALUE,           /**< Fin de la maquina virtual */
    BINARY_ADD,             /**< Suma */
    BINARY_SUBTRACT,        /**< Resta */
    BINARY_MULTIPLY,        /**< Multiplicacion */
    BINARY_FLOOR_DIVIDE,    /**< Division */
    BINARY_MODULO,          /**< Modulo */
    COMPARE_OP_LT,          /**< Operador < */
    COMPARE_OP_LTE,         /**< Operador <= */
    COMPARE_OP_GT,          /**< Operador > */
    COMPARE_OP_GTE,         /**< Operador >= */
    COMPARE_OP_EQ,          /**< Operador == */
    COMPARE_OP_NEQ,         /**< Operador != */
    POP_JUMP_IF_FALSE,
    POP_JUMP_IF_TRUE
} lat_ins;


/**\brief Estructura que almacena las instrucciones bytecode de la MV */
typedef struct lat_bytecode
{
    lat_ins ins;    /**< Instruccion */
    void* a;        /**< datos a */
    void* b;        /**< datos b */
    void* c;        /**< datos c */
} lat_bytecode;

/**\brief Define una funcion de usuario */
typedef struct lat_funcion
{
    int num_params;         /**< Numero de argumentos para la funcion */
    list_node* params;      /**< Parametros para la funcion */
    lat_bytecode* bcode;    /**< Instrucciones de la funcion */
    //lat_objeto *closure;
} lat_funcion;

/**\brief Define la maquina virtual (MV) */
struct lat_mv
{
    list_node* pila;     /**< pila de la maquina virtual */
    list_node* modulos;     /**< modulos importados en la MV */
    list_node* todos_objetos;     /**< objetos creados dinamicamente en la MV */
    list_node* basurero_objetos;     /**< objetos listos para liberar por el colector de basura */
    lat_objeto* registros[8];    /**< Registros auxiliares de la MV */
    lat_objeto* contexto_pila[256];   /**< Tabla hash para el contexto actual */
    lat_objeto* objeto_cierto;   /**< Valor logico verdadero */
    lat_objeto* objeto_falso;   /**< Valor logico falso */
    size_t memoria_usada;      /**< Tamanio de memoria creado dinamicamente */
    int apuntador_pila;      /**< Apuntador de la pila */
    bool REPL;  /**< Indica si esta corriendo REPL */
};

/**\brief Crea la maquina virtual (MV)
  *
  *\return lat_mv: Apuntador a la MV
  */
lat_mv* lat_crear_maquina_virtual();

/**\brief Ejecuta una cadena de instrucciones en la MV
  *
  *\param vm: Apuntador a la MV
  */
void lat_ejecutar(lat_mv *mv);

/**\brief Ejecuta un archivo en la MV
  *
  *\param vm: Apuntador a la MV
  */
void lat_ejecutar_archivo(lat_mv *mv);

/**\brief Inserta un objeto en la pila de la MV
  *
  *\param vm: Apuntador a la MV
  *\param o: Apuntador a objeto
  */
void lat_apilar(lat_mv *mv, lat_objeto* o);

/**\brief Extrae un objeto de la pila de la MV
  *
  *\param vm: Apuntador a la MV
  *\return lat_objeto: Apuntador a objeto
  */
lat_objeto* lat_desapilar(lat_mv *mv);

/**\brief Inserta un objeto al final de la lista
  *
  *\param lista: Apuntador a la lista
  *\param o: Apuntador a objeto
  */
void lat_apilar_lista(lat_objeto* lista, lat_objeto* o);

/**\brief Extrae el ultimo elemento de la lista
  *
  *\param lista: Apuntador a la lista
  *\return lat_objeto: Apuntador a objeto
  */
lat_objeto* lat_desapilar_lista(lat_objeto* lista);

/**\brief Inserta un contexto en la pila de la MV
  *
  *\param vm: Apuntador a la MV
  */
void lat_apilar_contexto(lat_mv *mv);

/**\brief Extrae un contexto de la pila de la MV
  *
  *\param vm: Apuntador a la MV
  */
void lat_desapilar_contexto(lat_mv *mv);

/**\brief Inserta el contexto principal en la pila de la MV
  *
  *\param vm: Apuntador a la MV
  *\param ctx: Apuntador al contexto
  */
void lat_apilar_contexto_predefinido(lat_mv *mv, lat_objeto* ctx);

/**\brief Extrae el contexto principal de la pila de la MV
  *
  *\param vm: Apuntador a la MV
  *\return lat_objeto: Apuntador al contexto
  */
lat_objeto* lat_desapilar_contexto_predefinido(lat_mv *mv);

/**\brief Extrae el contexto de la pila de la MV
  *
  *\param vm: Apuntador a la MV
  *\return lat_objeto: Apuntador al contexto
  */
lat_objeto* lat_obtener_contexto(lat_mv *mv);

/**\brief Agrega un objeto al basurero para ser destruido
  *
  *\param vm: Apuntador a la MV
  *\param o: Apuntador a objeto
  */
void lat_basurero_agregar(lat_mv *mv, lat_objeto* o);

/**\brief Crea un objeto basurero
  *
  *\param vm: Apuntador a la MV
  */
void lat_basurero(lat_mv *mv);

/**\brief Define una funcion creada por el usuario
  *
  *\param vm: Apuntador a la MV
  *\param inslist: Lista de instrucciones de la funcion
  *\return lat_objeto: Apuntador a un objeto tipo funcion
  */
lat_objeto* lat_definir_funcion(lat_mv *mv, lat_bytecode* inslist, int num_params);

/**\brief Define una funcion creada en C
  *
  *\param vm: Apuntador a la MV
  *\param *function: Apuntador a la funcion definida en C
  *\return lat_objeto: Apuntador a un objeto tipo cfuncion
  */
lat_objeto* lat_definir_cfuncion(lat_mv *mv, void (*function)(lat_mv *mv));

/**\brief Regresa el numero en que se encuentra un elemento
  *
  *\param vm: Apuntador a la MV
  */
void lat_numero_lista(lat_mv *mv);

/**\brief Envia a consola el contenido de la lista
  *
  *\param vm: Apuntador a la MV
  *\param l: Apuntador a la lista
  */
void lat_imprimir_lista(lat_mv *mv, list_node* l);

/**\brief Envia a consola el contenido del diccionario
  *
  *\param vm: Apuntador a la MV
  *\param d: Apuntador al diccionario
  */
void lat_imprimir_diccionario(lat_mv *mv, hash_map* d);

/**\brief Envia a consola el valor del objeto
  *
  *\param vm: Apuntador a la MV
  */
void lat_imprimir(lat_mv *mv);

/**\brief Clona un objeto
  *
  *\param vm: Apuntador a la MV
  */
void lat_clonar(lat_mv *mv);

/**\brief Agrega un elemento a la lista
  *
  *\param vm: Apuntador a la MV
  */
void lat_cons(lat_mv *mv);

/**\brief Operador +
  *
  *\param vm: Apuntador a la MV
  */
void lat_sumar(lat_mv *mv);

/**\brief Operador -
  *
  *\param vm: Apuntador a la MV
  */
void lat_restar(lat_mv *mv);

/**\brief Operador *
  *
  *\param vm: Apuntador a la MV
  */
void lat_multiplicar(lat_mv *mv);

/**\brief Operador /
  *
  *\param vm: Apuntador a la MV
  */
void lat_dividir(lat_mv *mv);

/**\brief Operador %
  *
  *\param vm: Apuntador a la MV
  */
void lat_modulo(lat_mv *mv);

/**\brief Operador !=
  *
  *\param vm: Apuntador a la MV
  */
void lat_diferente(lat_mv *mv);

/**\brief Operador ==
  *
  *\param vm: Apuntador a la MV
  */
void lat_igualdad(lat_mv *mv);

/**\brief Operador <
  *
  *\param vm: Apuntador a la MV
  */
void lat_menor_que(lat_mv *mv);

/**\brief Operador <=
  *
  *\param vm: Apuntador a la MV
  */
void lat_menor_igual(lat_mv *mv);

/**\brief Operador <
  *
  *\param vm: Apuntador a la MV
  */
void lat_mayor_que(lat_mv *mv);

/**\brief Operador >=
  *
  *\param vm: Apuntador a la MV
  */
void lat_mayor_igual(lat_mv *mv);

/**\brief Operador &&
  *
  *\param vm: Apuntador a la MV
  */
void lat_y(lat_mv *mv);

/**\brief Operador ||
  *
  *\param vm: Apuntador a la MV
  */
void lat_o(lat_mv *mv);

/**\brief Operador !
  *
  *\param vm: Apuntador a la MV
  */
void lat_negacion(lat_mv *mv);

/**\brief Obtiene el tipo de dato en cadena
  *
  *\param vm: Apuntador a la MV
  */
void lat_tipo(lat_mv *mv);

/**\brief Convierte un valor a logico
  *
  *\param vm: Apuntador a la MV
  */
void lat_logico(lat_mv *mv);

/**\brief Convierte un valor a entero
  *
  *\param vm: Apuntador a la MV
  */
void lat_entero(lat_mv *mv);

/**\brief Convierte un valor a literal
  *
  *\param vm: Apuntador a la MV
  */
void lat_literal(lat_mv *mv);

/**\brief Convierte un valor a decimal
  *
  *\param vm: Apuntador a la MV
  */
void lat_decimal(lat_mv *mv);

/**\brief Convierte un valor a cadena
  *
  *\param vm: Apuntador a la MV
  */
void lat_cadena(lat_mv *mv);

/**\brief Obtiene el maximo de dos numeros
  *
  *\param vm: Apuntador a la MV
  */
void lat_maximo(lat_mv *mv);

/**\brief Obtiene el minimo de dos numeros
  *
  *\param vm: Apuntador a la MV
  */
void lat_minimo(lat_mv *mv);

/**\brief Determina si una cadena es entero
  *
  *\param vm: Apuntador a la MV
  */
void lat_es_entero(lat_mv *mv);

/**\brief Determina si una cadena es decimal
  *
  *\param vm: Apuntador a la MV
  */
void lat_es_decimal(lat_mv *mv);

/**\brief Formatea un decimal con el numero de caracteres decimales que se especifique
  *
  *\param vm: Apuntador a la MV
  */
void lat_formato_numero(lat_mv *mv);

/**\brief Sale del sistema de Latino REPL
  *
  *\param vm: Apuntador a la MV
  */
void lat_salir(lat_mv *mv);

/**\brief Crea un objeto bytecode
  *
  *\param i: Tipo de instruccion
  *\param a: Registro a
  *\param b: Registro b
  *\param meta: Datos
  *\return lat_bytecode: Objeto bytecode
  */
lat_bytecode lat_bc(lat_ins i, void* a, void* b, void* meta);

/**\brief Ejecuta una funcion
  *
  *\param vm: Apuntador a la MV
  *\param func: Apuntador a funcion a ejecutar
  */
lat_objeto* lat_llamar_funcion(lat_mv *mv, lat_objeto* func);

#endif //_VM_H_
