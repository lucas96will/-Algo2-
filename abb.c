#include "abb.h"
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

typedef struct nodo nodo_t;

struct nodo {
    nodo_t* derecha;
    nodo_t* izquierda;
    void* dato;
    char* clave;
};

struct abb {
    nodo_t* raiz;
    abb_destruir_dato_t f_destruir;
    abb_comparar_clave_t f_comparar;
    size_t cantidad;
};

typedef struct par_padre_hijo {
    nodo_t* padre;
    nodo_t* elem_a_borrar;
} par_padre_hijo_t;



/* *****************************************************************
 *                    FUNCIONES DEL NODO
 * *****************************************************************/

nodo_t* nodo_crear(char* clave, void* dato) {
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
    if(f_destruir != NULL) {
        f_destruir(nodo->dato);
    }
    free(nodo->clave);
    free(nodo);
}



/* *****************************************************************
 *                    FUNCIONES AUXILIARES DEL ABB
 * *****************************************************************/

nodo_t *abb_obtener_nodo_misma_clave(nodo_t *nodo, const char *clave, abb_comparar_clave_t f_comparar) {
    if(nodo == NULL) {
        return NULL;
    }

    if(f_comparar(nodo->clave, clave) == 0) {
        return nodo;
    }
    if(f_comparar(nodo->clave, clave) > 0) {
        // La clave es mayor ? -> voy al subarbol izquierdo
        return abb_obtener_nodo_misma_clave(nodo->izquierda, clave, f_comparar);
    }
    else { // La clave es menor, por lo que busco a la derecha
        return abb_obtener_nodo_misma_clave(nodo->derecha, clave, f_comparar);
    }
} // Complejidad: T(n) = T(n/2) + O(1) => complejidad O(log(n))  (Asumo que en el peor de los casos el arbol esta balanceado)

par_padre_hijo_t* par_padre_hijo_crear(nodo_t* padre, nodo_t* hijo) {
    par_padre_hijo_t* par_padre_hijo = malloc(sizeof(par_padre_hijo_t));
    if (par_padre_hijo == NULL){
        return NULL;
    }

    par_padre_hijo->padre = padre;
    par_padre_hijo->elem_a_borrar = hijo;

    return par_padre_hijo;
}

void par_padre_hijo_destruir(par_padre_hijo_t* par_padre_hijo, abb_destruir_dato_t f_destruir) {

    nodo_destruir(par_padre_hijo->padre, f_destruir);
    nodo_destruir(par_padre_hijo->elem_a_borrar, f_destruir);

    free(par_padre_hijo);
}

par_padre_hijo_t* abb_obtener_elem_a_borrar(nodo_t *padre, nodo_t* hijo, const char *clave, abb_comparar_clave_t f_comparar){
    if(hijo == NULL) {
        return NULL;
    }

    if(f_comparar(hijo->clave, clave) == 0) {
        par_padre_hijo_t* par_padre_hijo = par_padre_hijo_crear(padre, hijo);
        return par_padre_hijo;
    }
    if(f_comparar(hijo->clave, clave) > 0) {
        // La clave es mayor ? -> voy al subarbol izquierdo
        return abb_obtener_elem_a_borrar(hijo, hijo->izquierda, clave, f_comparar);
    }
    else { // La clave es menor, por lo que busco a la derecha
        return abb_obtener_elem_a_borrar(hijo, hijo->derecha, clave, f_comparar);
    }
} // Complejidad: T(n) = T(n/2) + O(1) => complejidad O(log(n))  (Asumo que en el peor de los casos el arbol esta balanceado)

char* busqueda_reemplazante(nodo_t* nodo) { //Menor de sus hijos mayores
    nodo_t* actual = nodo->derecha;

    while (actual->izquierda != NULL) {
        actual = actual->izquierda;
    }

    return actual->clave;
}
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


bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    nodo_t* actual = arbol->raiz;
    while (true){
        if (actual == NULL) {
            char* copia = strdup(clave);
            nodo_t* nuevo_nodo = nodo_crear(copia, dato);
            actual = nuevo_nodo;
            arbol->cantidad++;
            return true;
        }
        if(f_comparar(actual->clave, clave) == 0) {
            if(arbol->f_destruir != NULL) {
                f_destruir(actual->dato);
            }
            actual->dato = dato;
            return true;
        }
        if(f_comparar(actual->clave, clave) > 0) {
        // La clave es mayor ? -> voy al subarbol izquierdo
            actual = actual->izquierda;
            continue;
        }
        if(f_comparar(actual->clave, clave) < 0) {
        // La clave es menor ? -> voy al subarbol derecho
            actual = actual->derecha;
            continue;
        }
    }
}



void *abb_borrar(abb_t *arbol, const char *clave) {
    par_padre_hijo_t* par_padre_hijo = abb_obtener_elem_a_borrar(NULL, arbol->raiz, clave, arbol->f_comparar);

    if (!par_padre_hijo->elem_a_borrar->izquierda && !par_padre_hijo->elem_a_borrar->derecha ) { //Elem a borrar no tiene hijos
        if(f_comparar(par_padre_hijo->padre->clave, par_padre_hijo->elem_a_borrar->clave) > 0) {
            par_padre_hijo->padre->izquierda = NULL;
        } else {
            par_padre_hijo->padre->derecha = NULL;
        }
        free(par_padre_hijo->elem_a_borrar->clave);
        arbol->cantidad--;
        return par_padre_hijo->elem_a_borrar->dato;
    }

    if (par_padre_hijo->elem_a_borrar->izquierda && !par_padre_hijo->elem_a_borrar->derecha ) { //Elem a borrar tiene hijo izq
        if(f_comparar(par_padre_hijo->padre->clave, par_padre_hijo->elem_a_borrar->clave) > 0) {
            par_padre_hijo->padre->izquierda = par_padre_hijo->elem_a_borrar->izquierda;
        } else {
            par_padre_hijo->padre->derecha = par_padre_hijo->elem_a_borrar->izquierda;
        }
        free(par_padre_hijo->elem_a_borrar->clave);
        arbol->cantidad--;
        return par_padre_hijo->elem_a_borrar->dato;
    }

    if (!par_padre_hijo->elem_a_borrar->izquierda && par_padre_hijo->elem_a_borrar->derecha ) { //Elem a borrar tiene hijo der
        if(f_comparar(par_padre_hijo->padre->clave, par_padre_hijo->elem_a_borrar->clave) > 0) {
            par_padre_hijo->padre->izquierda = par_padre_hijo->elem_a_borrar->derecha;
        } else {
            par_padre_hijo->padre->derecha = par_padre_hijo->elem_a_borrar->derecha;
        }
        free(par_padre_hijo->elem_a_borrar->clave);
        arbol->cantidad--;
        return par_padre_hijo->elem_a_borrar->dato;
    }

    if (par_padre_hijo->elem_a_borrar->izquierda && par_padre_hijo->elem_a_borrar->derecha ) { //Elem a borrar tiene dos hijos
        char* clave_reemp = busqueda_reemplazante(par_padre_hijo->elem_a_borrar);
        void* dato_reemp = abb_borrar(arbol, clave_reemp);
        void* dato_borrado = par_padre_hijo->elem_a_borrar->dato;
        free(par_padre_hijo->elem_a_borrar->clave);
        par_padre_hijo->elem_a_borrar->clave = clave_reemp;
        par_padre_hijo->elem_a_borrar->dato = dato_reemp;
        return;
    }
}


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


/* *****************************************************************
 *                    ITER INTERNO DEL ABB
 * *****************************************************************/

void abb_in_order(abb_t *arbol, bool visitar(const char *clave, void *dato, void *extra), void *extra) {
    //Continuara..