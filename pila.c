#include "pila.h"

#include <stdlib.h>

#define CAPACIDAD 10
#define MULT_AGRANDAMIENTO 2
#define MULT_ACHICAMIENTO 4

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

// Redimension para pila, no es una primitiva:
bool pila_redimensionar(pila_t *pila, size_t capacidad_nueva) {
    if (capacidad_nueva < 1) {
        capacidad_nueva = 1;
    }
    void **datos_nuevos = realloc(pila->datos, capacidad_nueva * sizeof(void*) );
    if (datos_nuevos == NULL) {
        return false;
    }
    pila->capacidad = capacidad_nueva;
    pila->datos = datos_nuevos;
    return true;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void) {
    pila_t *pila = malloc(sizeof(pila_t));

    if (pila == NULL) {
        return NULL;
    }

    pila->datos = malloc(CAPACIDAD * sizeof(void*));

    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }

    pila->cantidad = 0;
    pila->capacidad = CAPACIDAD;
    
    return pila;
}


void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
    pila = NULL;
}


bool pila_esta_vacia(const pila_t *pila) {
    if(pila->cantidad > 0) {
        return false;
    }
    return true;
}


bool pila_apilar(pila_t *pila, void *valor) {
    if (pila->cantidad == pila->capacidad) {
        size_t capacidad_nueva = pila->capacidad * MULT_AGRANDAMIENTO;
        if (pila_redimensionar(pila, capacidad_nueva) == false) {
            return false;
        }
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}


void *pila_ver_tope(const pila_t *pila) {
    if (pila_esta_vacia(pila)) {
        return NULL;
    }
    size_t pos_tope = pila->cantidad - 1;
    return pila->datos[pos_tope];
}


void *pila_desapilar(pila_t *pila) {
    if (pila_esta_vacia(pila)) {
        return NULL;
    }
    void* tope = pila_ver_tope(pila);
    pila->cantidad--;

    if (pila->cantidad * MULT_ACHICAMIENTO <= pila->capacidad) {
        size_t capacidad_nueva = pila->capacidad / MULT_AGRANDAMIENTO;
        pila_redimensionar(pila, capacidad_nueva);
    }
    
    return tope;
}
