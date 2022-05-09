#include "abb.h"
#include <stdlib.h>

typedef struct nodo nodo_t;

struct nodo {
    nodo_t* derecha;
    nodo_t* izquierda;
    void* dato;
    void* clave;
};

struct abb {
    nodo_t* raiz;
    abb_destruir_dato_t f_destruir;
    abb_comparar_clave_t f_comparar;
    size_t cantidad;
};

/* *****************************************************************
 *                    PRIMITIVAS DEL NODO
 * *****************************************************************/

nodo_t* nodo_crear(void* clave, void* dato) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(nodo == NULL) {
        return NULL;
    }

    nodo->derecha = NULL;
    nodo->izquierda = NULL;
    nodo->clave = clave;
    nodo->dato = dato;

    return nodo;
}

void nodo_destruir(nodo_t* nodo, abb_destruir_dato_t f_destruir) {
    if(f_destruir == NULL) {
        free(nodo->clave);
    } else {
        f_destruir(nodo->clave);
        f_destruir(nodo->dato);
    }
    free(nodo);
}



/* *****************************************************************
 *                    FUNCIONES AUXILIARES DEL ABB
 * *****************************************************************/

void *abb_obtener_nodo_misma_clave(nodo_t *nodo, const char *clave, abb_comparar_clave_t f_comparar) {
    if(nodo == NULL) {
        return NULL;
    }

    if(f_comparar(nodo->clave, clave) == 0) {
        return nodo;
    }
    if(f_comparar(nodo->clave, clave) > 0) {
        // La clave es mayor ? -> voy al subarbol derecho
        return abb_obtener_nodo_misma_clave(nodo->derecha, clave, f_comparar);
    }
    else { // La clave es menor, por lo que busco a la izquierda
        return abb_obtener_nodo_misma_clave(nodo->izquierda, clave, f_comparar);
    }
} // Complejidad: T(n) = T(n/2) + O(1) => complejidad O(log(n))  (Asumo que en el peor de los casos el arbol esta balanceado)


/* *****************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t* abb = malloc(sizeof(abb_t));
    if(abb == NULL) {
        return NULL;
    }

    abb->raiz = NULL;
    abb->cantidad = 0;
    abb->f_destruir = destruir_dato;
    abb->f_comparar = cmp;

    return abb;
}


bool abb_guardar(abb_t *arbol, const char *clave, void *dato);


void *abb_borrar(abb_t *arbol, const char *clave);


void *abb_obtener(const abb_t *arbol, const char *clave) {
    nodo_t* nodo = abb_obtener_nodo_misma_clave(arbol->raiz, clave, arbol->f_comparar);
    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    if(abb_obtener_nodo_misma_clave(arbol->raiz, clave, arbol->f_comparar) == NULL) {
        return false;
    }
    return true;
}

size_t abb_cantidad(const abb_t *arbol) {
    return arbol->cantidad;
}

void abb_destruir_nodos(nodo_t* nodo, abb_destruir_dato_t f_destruir) {
    if(nodo == NULL) {
        return;
    }
    abb_destruir_nodos(nodo->derecha, f_destruir);
    abb_destruir_nodos(nodo->izquierda, f_destruir);
    nodo_destruir(nodo, f_destruir);
}


void abb_destruir(abb_t *arbol) {
    abb_destruir_nodos(arbol->raiz, arbol->f_destruir);
    free(arbol);
}