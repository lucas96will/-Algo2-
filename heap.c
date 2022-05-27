#include "heap.h"

#define FACTOR_REDIMENSION 2
#define TAM_INICIAL 20
#define MULTIPLO_CANTIDAD 4

typedef struct heap {
    void** datos;
    size_t cantidad;
    size_t capacidad;
    cmp_func_t cmp;
} heap_t;

/* ******************************************************************
 *            DEFINICION FUNCIONES AUXILIARES PARA HEAP
 * *****************************************************************/

/*
 * Pre:
 * Post:
 */
bool down_heap(void* arreglo[], size_t tam, cmp_func_t cmp, size_t pos);

/*
 * Pre:
 * Post:
 */
bool up_heap(void* arreglo[], size_t tam, cmp_func_t cmp, size_t pos);

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

bool down_heap(void* arreglo[], size_t tam, cmp_func_t cmp, size_t pos) {

}


bool up_heap(void* arreglo[], size_t tam, cmp_func_t cmp, size_t pos) {

}

void heapify(void* arreglo[], size_t tam, cmp_func_t cmp) {

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

    heap->cantidad = 0;
    heap->capacidad = TAM_INICIAL;
    heap->cmp = cmp;

    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    if(!cmp) return NULL;

    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;

    heap->datos = malloc(sizeof(void*) * (n * FACTOR_REDIMENSION));
    if(!heap->datos) {
        free(heap);
        return NULL;
    }
    heap->capacidad = n * FACTOR_REDIMENSION;
    heap->cantidad = n;
    heap->cmp = cmp;

    for(int i = 0; i < n; i++) {
        heap->datos[i] = arreglo[i];
    }

    heapify(heap->datos, heap->cantidad, cmp);

    return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {
    if(destruir_elemento != NULL) {
        for(int i = 0; i < heap->cantidad; i++) {
            destruir_elemento(heap->datos[i]);
        }
    }
    free(heap->datos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap) {
    return (heap_cantidad(heap) > 0);
}

bool heap_encolar(heap_t *heap, void *elem) {

    // Agrego al final
    heap->datos[heap->cantidad] = elem;
    heap->cantidad++;

    if(heap->cantidad == heap->capacidad) {
        if(!heap_redimensionar(heap, FACTOR_REDIMENSION * heap->capacidad)) {
            return false;
        }
    }

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
    void* dato = heap->datos[0];
    heap->datos[0] = heap->datos[heap->cantidad];
    heap->datos[heap->cantidad] = NULL;

    // Disminuyo en 1 la cantidad
    heap->cantidad--;

    // Hago downheap del primer elemento
    down_heap(heap->datos, heap->cantidad, heap->cmp, 0);

    // Redimensiono si es el caso

    if(MULTIPLO_CANTIDAD*heap->cantidad <= heap->capacidad && heap->cantidad != 0) {
        heap_redimensionar(heap, heap->capacidad / FACTOR_REDIMENSION);
    }

    return dato;
}
