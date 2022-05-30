#ifndef _ALGO2__HEAP_H
#define _ALGO2__HEAP_H

#include <stdbool.h>
#include <stdlib.h>

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct heap heap_t;

typedef int (*cmp_func_t) (const void *a, const void *b);

/* *****************************************************************
 *                    PRIMITIVAS DEL HEAP
 * *****************************************************************/

/*
 * Pre: La función de comparación (de tipo cmp_func_t) debe recibir
 * dos punteros del tipo de dato utilizado en el heap, y debe devolver:
 * menor a 0 si a < b ; 0 si a == b ; mayor a 0 si a > b
 * Post: Se devuelve el heap creado, si no se pudo crear devuelve NULL
 */
heap_t *heap_crear(cmp_func_t cmp);

/* Se crea un heap a partir de un datos y una funcion de comparacion
 * Pre: La función de comparación (de tipo cmp_func_t) debe ser correcta
 * Post: Se devuelve el heap creado, si no se pudo crear devuelve NULL
 */
heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp);

/*
 * Pre: el heap fue creado.
 * Post: Destruye la estructura del heap. si destruir_elemento no
 * es NULL, se aplica destruir_elemento a cada dato del heap
 */
void heap_destruir(heap_t *heap, void destruir_elemento(void *e));

/*
 * Pre: El heap fue creado
 * Post: Devuelve la cantidad de elementos almacenados en el heap
 */
size_t heap_cantidad(const heap_t *heap);

/*
 * Pre: El heap fue creado
 * Post: Devuelve true si el heap tiene mayor a 0 elementos
 * false en otro caso
 */
bool heap_esta_vacio(const heap_t *heap);

/*
 * Pre: El heap fue creado
 * Post: Se agrego el elemento en el heap
 * si no se pudo encolar devuelve false
 */
bool heap_encolar(heap_t *heap, void *elem);

/*
 * Pre: El heap fue creado
 * Post: Se devuelve el primer elemento del heap, si no se pudo devuelve NULL
 */
void *heap_ver_max(const heap_t *heap);

/*
 * Pre: El heap fue creado
 * Post: Se elimina el primer elemento del heap y se devuelve
 * si no se pudo desencolar devuelve NULL
 */
void *heap_desencolar(heap_t *heap);

/*
 * Pre: La funcion de comparacion es valida
 * Post: el datos elementos[] queda ordenado de manera
 * usando la funcion de comparacion de cualquier elemento y su
 * siguiente, siga la validez de la misma
 */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp);


/* *****************************************************************
 *                    PRUEBAS PARA LA LISTA
 * *****************************************************************/

// Realiza pruebas sobre la implementación realizada.
void pruebas_heap_estudiante(void);


#endif //_ALGO2__HEAP_H