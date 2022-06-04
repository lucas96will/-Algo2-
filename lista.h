#ifndef TDA_LISTA_ENLAZADA_LISTA_H
#define TDA_LISTA_ENLAZADA_LISTA_H

#include <stdbool.h>
#include <stdlib.h>


/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* Breve descripción */

typedef struct lista lista_t;

typedef struct lista_iter lista_iter_t;

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: se devolvió una nueva lista vacia.
lista_t *lista_crear(void);

// Devuelve true si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada. La lista no se modifica.
bool lista_esta_vacia(const lista_t *lista);

// Inserta un elemento que pasa a ser el primero de la lista. Devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: se insertó un nuevo elemento a la lista, que se encuentra al inicio de la misma.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta un elemento que pasa a ser el ultimo de la lista. Devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: se insertó un nuevo elemento a la lista, que se encuentra al final de la misma.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Elimina el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior(el eliminado), la lista
// contiene un elemento menos, si la lista no estaba vacía. La lista no se modifica.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía. La lista no se modifica.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía. La lista no se modifica.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve el largo de la lista, cero si esta vacia.
// Pre: la lista fue creada.
// Post: se devolvió el largo de la lista. La lista no se modifica.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/* *****************************************************************
 *               PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

// Iterarador interno de la lista. Recorre la lista hasta el final, aplicando la funcion visitar
// a cada elemento o, mientras la función visitar devuelva true.
// Pre: la lista fue creada.
// Post: Se recorrió la lista hasta el final, o hasta que la función visitar devolvió true.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* *****************************************************************
 *               PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// El iterador es una estructura que permite recorrer los elementos de la lista
// Pre: la lista fue creada
// Post: Se devuelve el iterador en la primera posicion de la lista
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza el iterador una posicion en la lista
// la posicion que puede tomar el iterador es desde el principio
// hasta el final de la lista
// Pre: el iterador fue creado
// Post: Devuelve true si se pudo avanzar el iterador en la lista
// en otro caso, false
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene el valor en el cual el iterador esta posicionado en la lista
// Pre: el iterador fue creado
// Post: devuelve el valor del iterador o NULL si la lista
// no tiene elementos o el iterador esta al final de la lista
void *lista_iter_ver_actual(const lista_iter_t *iter);


// Pre: el iter fue creado
// Post: devuelve true si el iterador se encuentra al final, false en otro caso
bool lista_iter_al_final(const lista_iter_t *iter);

// Destructor del iterador externo.
// Pre: el iterador fue creado
// Post: Se libera la memoria del iterador
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento en la lista en la posicion del iterador
// Pre: el iterador fue creado
// Post: El elemento insertado va a tomar la posicion del iterador
// Como resultado ver el iterador actual es el elemento agregado
// Devuelve true si se inserto el elemento en la lista
// false en otro caso
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Elimina el elemento en el cual esta posicionado el iterador.
// Si la lista no tiene elementos o si el iterador se encuentra
// al final, devuelve NULL
// Pre: el iter fue creado
// Post: Se elimino el elemento de la lista y se devuelve el valor.
// Si no se pudo borrar se devuelve NULL
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                    PRUEBAS PARA LA LISTA
 * *****************************************************************/

// Realiza pruebas sobre la implementación realizada.
void pruebas_lista_estudiante(void);


#endif //TDA_LISTA_ENLAZADA_LISTA_H