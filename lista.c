#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

/* Definición del struct nodo*/
typedef struct nodo {
    void *dato;
    struct nodo *siguiente;
} nodo_t;

/* Definición del struct lista*/
struct lista {
    nodo_t *primero;
    nodo_t *ultimo;
    size_t largo;
};

/* Definición del struct lista_iter*/
struct lista_iter {
	lista_t *lista;
	nodo_t *act;
	nodo_t *ant;
};

/* ******************************************************************
 *                FUNCIONES AUXILIARES PARA NODOS
 * *****************************************************************/

nodo_t *crear_nodo(void* dato);
void destruir_nodo(nodo_t *nodo);

nodo_t *crear_nodo(void* dato) {
    nodo_t *nodo= malloc(sizeof(nodo_t));
    if (nodo == NULL) {
        return NULL;
    }
    nodo->dato = dato;
    nodo->siguiente = NULL;
    return nodo;
}

void destruir_nodo(nodo_t *nodo) {
    free(nodo);
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t *lista_crear(void) {
    lista_t *lista = malloc(sizeof(lista_t));
    if (lista == NULL) {
        return lista;
    }

    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->largo = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    if (lista->largo == 0 && lista->primero == NULL && lista->ultimo == NULL){
        return true;
    }
    return false;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t *nodo_nuevo = crear_nodo(dato);
    if (nodo_nuevo == NULL) {
        return false;
    }
    if (lista_esta_vacia(lista)) {
        lista->ultimo  = nodo_nuevo;
    } else {
        nodo_nuevo->siguiente = lista->primero;
    }
    lista->primero = nodo_nuevo;
    lista->largo++;
    return true;

}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    nodo_t *nodo_nuevo = crear_nodo(dato);
    if (nodo_nuevo == NULL) {
        return false;
    }
    if (lista_esta_vacia(lista)) {
        lista->primero = nodo_nuevo;  
    } else {
        lista->ultimo->siguiente = nodo_nuevo;
    }
    lista->ultimo  = nodo_nuevo;
    nodo_nuevo->siguiente = NULL;
    lista->largo++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    nodo_t *primer_nodo = lista->primero;
    void *valor = primer_nodo->dato;

    if (primer_nodo->siguiente == NULL) {
        lista->primero = NULL;
        lista->ultimo = NULL;
    } else {
        lista->primero = primer_nodo->siguiente;
    }

    lista->largo--;
    destruir_nodo(primer_nodo);
    return valor;
}

void *lista_ver_primero(const lista_t *lista) {
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t *lista) {
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
    while (!lista_esta_vacia(lista)) {
        void *elemento_eliminado = lista_borrar_primero(lista);
         if (destruir_dato != NULL){
            destruir_dato(elemento_eliminado);
        }
    }
    free(lista);    
}

/* *****************************************************************
 *               PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    if (lista_esta_vacia(lista)) {
        return;
    }

    bool continuar = true;
    nodo_t *actual = lista->primero;
    
    while (actual && continuar) {

        continuar = visitar(actual->dato, extra);
        actual = actual->siguiente;
    }
}

/* *****************************************************************
 *               PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t *iter = malloc(sizeof(lista_iter_t));
    if(iter == NULL){
        return NULL;
    }
    
    iter->act = lista->primero;
    iter->lista = lista;
    iter->ant = NULL;

    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    // Si la lista esta vacia o si el iter se encuentra al final, no se puede avanzar
    if(lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)) {
        return false;
    }

    iter->ant = iter->act;
    iter->act = iter->act->siguiente;

    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    if(lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)) {
        return NULL;
    }

    return iter->act->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
    return (iter->act == NULL && iter->ant == iter->lista->ultimo);
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    nodo_t *nodo_nuevo = crear_nodo(dato);
    if(nodo_nuevo == NULL) {
        return false;
    }
    nodo_nuevo->siguiente = iter->act;

    // Lista vacia, inserto y cambio los punteros (primero y ultimo) de la lista
    if(lista_esta_vacia(iter->lista)) {
        iter->lista->primero = nodo_nuevo;
        iter->lista->ultimo = nodo_nuevo;
    } else if (lista_iter_al_final(iter)) { // Si el iterador esta al final, inserto y cambio el puntero ultimo de la lista
        iter->ant->siguiente = nodo_nuevo;
        iter->lista->ultimo = nodo_nuevo;
    } else if (iter->act == iter->lista->primero) { // Si el iterador esta en la primer posicion, inserto y cambio el puntero primero de la lista
        iter->lista->primero = nodo_nuevo;

    } else { // En otro caso el iterador no se encuentra ni al principio ni al final de la lista
        iter->ant->siguiente = nodo_nuevo;
        
    }
    iter->act = nodo_nuevo;
    iter->lista->largo++;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    if(lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)) {
        return NULL;
    }

    void *aux = iter->act->dato;
    nodo_t *nodo_eliminar = iter->act;

    if(iter->lista->largo == 1) { // Si la lista tiene solo 1 elemento
        iter->lista->ultimo = NULL;
        iter->lista->primero = NULL;
        iter->act = NULL;
    } else if(nodo_eliminar == iter->lista->ultimo) { // Si el iterador se encuentra en el ultimo elemento de la lista, es decir uno antes de lista_iter_al_final
        iter->lista->ultimo = iter->ant;
        iter->ant->siguiente = iter->act->siguiente;
        iter->act = iter->act->siguiente;
    } else if(iter->act == iter->lista->primero){ // Si el iterador se encuentra al principio de la lista
        iter->act = iter->act->siguiente;
        iter->lista->primero = iter->act;
    } else { // En otro caso el iterador se encuentra entre (sin incluir) el principio - final
        iter->act = iter->act->siguiente;
        iter->ant->siguiente = iter->act;
    }    

    destruir_nodo(nodo_eliminar);
    iter->lista->largo--;
    return aux;
}
