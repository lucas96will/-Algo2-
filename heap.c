#include "heap.h"
#include <stdio.h>
#include <string.h>

#define FACTOR_REDIMENSION 2
#define TAM_INICIAL 10
#define MULTIPLO_CANTIDAD 4


 struct heap {
    void** datos;
    size_t cantidad;
    size_t capacidad;
    cmp_func_t cmp;
};

/* ******************************************************************
 *            DEFINICION FUNCIONES AUXILIARES PARA HEAP
 * *****************************************************************/

/*
 * Pre:
 * Post:
 */
void down_heap(void* arreglo[], size_t tam, cmp_func_t cmp, size_t pos);

/*
 * Pre:
 * Post:
 */
void up_heap(void* arreglo[], size_t tam, cmp_func_t cmp, size_t pos);

/*
 * Pre:
 * Post:
 */
void heapify(void* arreglo[], size_t tam, cmp_func_t cmp);

/*
 * Pre: el heap fue creado y nueva_capacidad > 0
 * Post: hace realloc del arreglo heap->datos con la nueva capacidad
 * Devuelve true si el realloc fue exitoso y nueva_capacidad > 0 o false
 * en otro caso
 */
bool heap_redimensionar(heap_t *heap, size_t nueva_capacidad);


/* ******************************************************************
 *            IMPLEMENTACION FUNCIONES AUXILIARES PARA HEAP
 * *****************************************************************/

void swap(void** a, void** b) {
    void** aux = *a;
    *a = *b;
    *b = aux;

}


void down_heap(void* arreglo[], size_t tam, cmp_func_t cmp, size_t pos) {
    size_t h_izq = (pos*2) + 1;
    size_t h_der = (pos*2) + 2;

    // Si no hay hijos entonces no hay hijo izquierdo!
    if(h_izq >= tam) {
        return;
    }

    // Si no hay hijo derecho entonces comparo con el izquierdo (ultimo)
    if(h_der >= tam && cmp(arreglo[pos], arreglo[h_izq]) < 0) {
        swap(arreglo[pos], arreglo[h_izq]);
    }
    else if(h_der < tam) {

        // Comparo y devuelvo la pos del mayor entre los dos hijos
        size_t hijo_mayor = (cmp(arreglo[h_izq], arreglo[h_der]) < 0) ? h_der : h_izq;

        // Comparo si el padre es mayor que el hijo mayor
        if(cmp(arreglo[pos], arreglo[hijo_mayor]) < 0) {
            swap(arreglo[pos], arreglo[hijo_mayor]);
            down_heap(arreglo, tam, cmp, hijo_mayor);
        }
    }

}


void up_heap(void* arreglo[], size_t tam, cmp_func_t cmp, size_t pos) {
    if(pos == 0) return;
    size_t padre = (pos-1)/2;
    // Si no hay padre
    if(cmp(arreglo[padre], arreglo[pos]) >= 0) {
        return;
    }

    swap(&arreglo[padre], &arreglo[pos]);
    up_heap(arreglo, tam, cmp, padre);

}

void heapify(void* arreglo[], size_t tam, cmp_func_t cmp) {
    for(int i = ((int)tam-1)/2; i >= 0; i--) {
        down_heap(arreglo, tam, cmp, i);
    }
}

bool heap_redimensionar(heap_t *heap, size_t nueva_capacidad) {
    void** datos_nuevo = realloc(heap->datos, nueva_capacidad * sizeof(void*));
    if(nueva_capacidad > 0 && datos_nuevo == NULL){
        return false;
    }

    heap->datos = datos_nuevo;
    heap->capacidad = nueva_capacidad;
    return true;
}


/* ******************************************************************
 *                      PRIMITIVAS DEL HEAP
 * *****************************************************************/


heap_t *heap_crear(cmp_func_t cmp) {
    if(!cmp) return NULL;

    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;

    heap->datos = malloc(sizeof(void*) * TAM_INICIAL);
    if(!heap->datos){
        free(heap);
        return NULL;
    }

    for(int i = 0; i < TAM_INICIAL; i++) {
        heap->datos[i] = malloc(sizeof(void*));
    }

    heap->cantidad = 0;
    heap->capacidad = TAM_INICIAL;
    heap->cmp = cmp;

    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    if(!cmp) return NULL;

    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;

    heap->capacidad = n * FACTOR_REDIMENSION;
    heap->cantidad = n;
    heap->cmp = cmp;

    heap->datos = malloc(sizeof(void*) * heap->capacidad);
    if(!heap->datos) {
        free(heap);
        return NULL;
    }

    for(size_t i = 0; i < n; i++) {
        heap->datos[i] = malloc(sizeof(void*));
        memcpy(heap->datos[i], arreglo[i], sizeof(void*));
    }

    heapify(heap->datos, heap->cantidad, cmp);

    return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {
    for(size_t i = 0; i < heap->cantidad; i++) {
        if(destruir_elemento != NULL) {
            destruir_elemento(heap->datos[i]);
        } else {
            free(heap->datos[i]);
        }
    }
    free(heap->datos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap) {
    return (heap_cantidad(heap) == 0);
}

bool heap_encolar(heap_t *heap, void *elem) {
    if(heap->cantidad == heap->capacidad) {
        if(!heap_redimensionar(heap, FACTOR_REDIMENSION * heap->capacidad)) {
            return false;
        }
    }

    heap->datos[heap->cantidad] = malloc(sizeof(void*));
    if(!heap->datos[heap->cantidad]) return NULL;

    // Agrego al final
    memcpy(heap->datos[heap->cantidad], elem, sizeof(void*));
    heap->cantidad++;


    // Up_heap del ultimo elemento (cantidad - 1)
    up_heap(heap->datos, heap->cantidad, heap->cmp, heap->cantidad-1);

    return true;
}

void *heap_ver_max(const heap_t *heap) {
    if(heap_esta_vacio(heap)) {
        return NULL;
    }
    return heap->datos[0];
}

void *heap_desencolar(heap_t *heap) {
    if(heap_esta_vacio(heap)) return NULL;

    void* dato = malloc(sizeof(void*));
    if(!dato) return NULL;

    memcpy(dato, heap->datos[0], sizeof(void*));
    memcpy(heap->datos[0], heap->datos[heap->cantidad-1], sizeof(void*));

    // Disminuyo en 1 la cantidad
    heap->cantidad--;

    // Hago downheap del primer elemento si es que hay mas de 1 elemento!
    if(heap->cantidad > 1){
        down_heap(heap->datos, heap->cantidad, heap->cmp, 0);
    }

    free(heap->datos[heap->cantidad+1]);
    // Redimensiono si es el caso
   /* if(MULTIPLO_CANTIDAD*heap->cantidad <= heap->capacidad && heap->cantidad != 0) {
        heap_redimensionar(heap, heap->capacidad / FACTOR_REDIMENSION);
    }*/

    return dato;
}
