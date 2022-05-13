#define _POSIX_C_SOURCE 200809L

#include "abb.h"
#include "pila.h"

#include <stdlib.h>
#include <string.h>

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

struct abb_iter {
    pila_t* pila;
};

typedef struct par_padre_hijo {
    nodo_t* padre;
    nodo_t* elem_a_borrar;
} par_padre_hijo_t;



/* *****************************************************************
 *                    FUNCIONES DEL NODO
 * *****************************************************************/

nodo_t* nodo_crear(const char* clave, void* dato) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(nodo == NULL) {
        return NULL;
    }

    char* clave_copiada = strdup(clave);
    nodo->derecha = NULL;
    nodo->izquierda = NULL;
    nodo->clave = clave_copiada;
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

par_padre_hijo_t* par_padre_hijo_crear(nodo_t* padre, nodo_t* hijo) {
    par_padre_hijo_t* par_padre_hijo = malloc(sizeof(par_padre_hijo_t));
    if (par_padre_hijo == NULL){
        return NULL;
    }

    par_padre_hijo->padre = padre;
    par_padre_hijo->elem_a_borrar = hijo;

    return par_padre_hijo;
}

void par_padre_hijo_destruir(par_padre_hijo_t* par_padre_hijo) {

    free(par_padre_hijo);
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

bool _abb_guardar(abb_t* arbol, const char *clave, void*dato, nodo_t* padre, nodo_t* actual) {
    
    if (actual == NULL) {
            //char* copia = strdup(clave);
            nodo_t* nuevo_nodo = nodo_crear(clave, dato);
            if(!nuevo_nodo) {
                return false;
            }
            arbol->cantidad++;


        if(arbol->f_comparar(padre->clave, clave) > 0) {
        // La clave es mayor ? -> voy al hijo izquierdo
            padre->izquierda = nuevo_nodo;
            return true;
        } else {
            padre->derecha = nuevo_nodo;
            return true;
        }
    }

     if(arbol->f_comparar(actual->clave, clave) == 0) {
            if(arbol->f_destruir != NULL) {
                arbol->f_destruir(actual->dato);
            }
            actual->dato = dato;
            return true;
        }

    if(arbol->f_comparar(actual->clave, clave) > 0) {
    // La clave es mayor ? -> voy al subarbol izquierdo
        return _abb_guardar(arbol, clave, dato, actual, actual->izquierda);
    
    } else {
        return _abb_guardar(arbol, clave, dato, actual, actual->derecha);
    }
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

nodo_t* busqueda_reemplazante(nodo_t* nodo) { //Menor de sus hijos mayores
    nodo_t* actual = nodo->derecha;

    while (actual->izquierda != NULL) {
        actual = actual->izquierda;
    }

    return actual;
}

void pila_apilar_abb(pila_t* pila, nodo_t* nodo) {
    if (!nodo) {
        return;
    }

    pila_apilar(pila, nodo);
    pila_apilar_abb(pila, nodo->izquierda);
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
    if (!arbol->raiz) {
        //char* copia = strdup(clave);
        nodo_t* nuevo_nodo = nodo_crear(clave, dato);
        if(!nuevo_nodo) {
            return false;
        }
        arbol->raiz = nuevo_nodo;
        arbol->cantidad++;
        return true;
    }

    nodo_t* actual = arbol->raiz;

    return _abb_guardar(arbol, clave, dato, NULL, actual);
}

void *abb_borrar(abb_t *arbol, const char *clave) {
    if(!abb_pertenece(arbol, clave)) {
        return NULL;
    }
    par_padre_hijo_t* par_padre_hijo = abb_obtener_elem_a_borrar(NULL, arbol->raiz, clave, arbol->f_comparar);
    
    void* dato_borrado = NULL;

    if (!par_padre_hijo->elem_a_borrar->izquierda && !par_padre_hijo->elem_a_borrar->derecha ) { //Elem a borrar no tiene hijos
        if (!par_padre_hijo->padre){
            arbol->raiz= NULL;
        }
        else if(arbol->f_comparar(par_padre_hijo->padre->clave, par_padre_hijo->elem_a_borrar->clave) > 0) {
            par_padre_hijo->padre->izquierda = NULL;
        } else {
            par_padre_hijo->padre->derecha = NULL;
        }
        
        arbol->cantidad--;
        dato_borrado = par_padre_hijo->elem_a_borrar->dato;
        nodo_destruir(par_padre_hijo->elem_a_borrar, arbol->f_destruir);
        
    } else if  (par_padre_hijo->elem_a_borrar->izquierda && !par_padre_hijo->elem_a_borrar->derecha ) { //Elem a borrar tiene hijo izq
        
        if (!par_padre_hijo->padre) {
            arbol->raiz = par_padre_hijo->elem_a_borrar->izquierda;
        } else if(arbol->f_comparar(par_padre_hijo->padre->clave, par_padre_hijo->elem_a_borrar->clave) > 0) {
            par_padre_hijo->padre->izquierda = par_padre_hijo->elem_a_borrar->izquierda;
        } else {
            par_padre_hijo->padre->derecha = par_padre_hijo->elem_a_borrar->izquierda;
        }
        arbol->cantidad--;
        dato_borrado = par_padre_hijo->elem_a_borrar->dato;
        nodo_destruir(par_padre_hijo->elem_a_borrar, arbol->f_destruir);
        
    } else if (!par_padre_hijo->elem_a_borrar->izquierda && par_padre_hijo->elem_a_borrar->derecha ) { //Elem a borrar tiene hijo der

        if (!par_padre_hijo->padre) {
            arbol->raiz = par_padre_hijo->elem_a_borrar->derecha;
        } else if(arbol->f_comparar(par_padre_hijo->padre->clave, par_padre_hijo->elem_a_borrar->clave) > 0) {
            par_padre_hijo->padre->izquierda = par_padre_hijo->elem_a_borrar->derecha;
        } else {
            par_padre_hijo->padre->derecha = par_padre_hijo->elem_a_borrar->derecha;
        }
        arbol->cantidad--;
        dato_borrado = par_padre_hijo->elem_a_borrar->dato;
        nodo_destruir(par_padre_hijo->elem_a_borrar, arbol->f_destruir);
        
    } else if (par_padre_hijo->elem_a_borrar->izquierda && par_padre_hijo->elem_a_borrar->derecha ) { //Elem a borrar tiene dos hijos

        nodo_t* reemplazante = busqueda_reemplazante(par_padre_hijo->elem_a_borrar);
        char* clave_reemp = strdup(reemplazante->clave);
        void* dato_reemp = abb_borrar(arbol, clave_reemp);

        dato_borrado = par_padre_hijo->elem_a_borrar->dato;
        par_padre_hijo->elem_a_borrar->dato = dato_reemp;
        free(par_padre_hijo->elem_a_borrar->clave);
        par_padre_hijo->elem_a_borrar->clave = clave_reemp;
        
    }
    
    par_padre_hijo_destruir(par_padre_hijo);
    
    return dato_borrado;
}


void *abb_obtener(const abb_t *arbol, const char *clave) {
    if(!abb_pertenece(arbol, clave)) {
        return NULL;
    }
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

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {

    pila_t* pila = pila_crear();
    if (!pila) {
        return;
    }
    pila_apilar_abb(pila, arbol->raiz);
    bool continuar = true;

    nodo_t* actual = pila_desapilar(pila);

    while (actual && continuar) {
        continuar = visitar(actual->clave, actual->dato, extra);
        pila_apilar_abb(pila, actual->derecha);
        actual = (nodo_t*) pila_desapilar;
    }
    pila_destruir(pila);

}

/* *****************************************************************
 *                    ITER EXTERNO DEL ABB
 * *****************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if (!iter) {
        return NULL;
    }

    iter->pila = pila_crear();
    if(!iter->pila) {
        free(iter);
        return NULL;
    }

    nodo_t* actual = arbol->raiz;
    if (!actual){
        return NULL;
    }
    pila_apilar_abb(iter->pila, actual);
    
    return iter;
}


bool abb_iter_in_avanzar(abb_iter_t *iter) {
    if (!abb_iter_in_al_final(iter)) {
        return false;
    }
    nodo_t* actual = pila_desapilar(iter->pila);

    if (!actual->derecha) {
        return false;
    }

    pila_apilar_abb(iter->pila, actual->derecha);
    return true;
}


const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    nodo_t* tope = pila_ver_tope(iter->pila);
    return tope->clave;
}


bool abb_iter_in_al_final(const abb_iter_t *iter) {
    if (pila_esta_vacia(iter->pila)) {
        return true;
    }
    return false;
}


void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila);
    free(iter);
}