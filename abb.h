#ifndef _ALGO2__ABB_H
#define _ALGO2__ABB_H

#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdlib.h>

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

// tipo de función para comparar clave
typedef int (*abb_comparar_clave_t) (const char *, const char *);
// tipo de función para destruir dato
typedef void (*abb_destruir_dato_t) (void *);

/* *****************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

/*
 * Crea el ABB. Debe recibir una funcion de comparacion y una funcion para la destruccion de datos.
 * */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un elemento en el ABB, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura ABB fue inicializada
 * Post: Se almacenó el nodo con el par (clave, dato)
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento del ABB y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura ABB fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor de un elemento del ABB, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura ABB fue inicializada
 * Post: Devolvió el elemento corrrespondiente a la clave, en caso de encontrado.
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si clave pertenece o no al ABB.
 * Pre: La estructura hash fue inicializada
 * Post: Devuelve true si la clave pertenece, false en caso contrario.
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos del ABB.
 * Pre: La estructura ABB fue inicializada
 * Post: Devolvio la cantidad de elementos del ABB.
 */
size_t abb_cantidad(const abb_t *arbol);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada nodo con par (clave, dato).
 * Pre: La estructura ABB fue inicializada
 * Post: La estructura ABB fue destruida
 */
void abb_destruir(abb_t *arbol);


/* *****************************************************************
 *                    ITER INTERNO DEL ABB
 * *****************************************************************/
/*
* Iterador interno del ABB. Recorre la estructura in order hasta el final, mientras la funcion visitar devuelva true.
 * Pre: El ABB fue creado.
 * Post: Se recorrio el ABB hasta el final, o hasta que la funcion visitar devolvio false.
 * */
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);


/* *****************************************************************
 *                    ITER EXTERNO DEL ABB
 * *****************************************************************/
/*
* Crea el iterador externo del ABB, siguiendo un recorrido in order.
 * Pre: El ABB fue creado.
 * Post: Se devuelve el iterador en la primer posicion del ABB.
 * */
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/*
* Avanza a la siguiente posicion del ABB, siguiendo un recorrido inorder, mientras este no se encuentre al final.
 * Pre: El iterador fue creado.
 * Post: Devuelve true si pudo avanzar, false en caso contrario.
 * */
bool abb_iter_in_avanzar(abb_iter_t *iter);

/*
* Devuelve la clave en la que se encuentra posicionado el iterador.
 * Pre: El iterador fue creado.
 * Post: Se devolvio la clave de la posicion actual del iterador.
 * */
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/*
* Devuelve true si el iterador se encuentra al final, false en caso contrario.
 * Pre: El iterador fue creado.
 * Post: Devolvio true si esta al final, false en caso contrario.
 * */
bool abb_iter_in_al_final(const abb_iter_t *iter);

/*
* Destruye el iterador.
 * Pre: El iterador fue creado.
 * Post: El iterador fue destruido.
 * */
void abb_iter_in_destruir(abb_iter_t* iter);


#endif //_ALGO2__ABB_H