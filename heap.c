#include "heap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define FACTOR_REDIMENSION 2
#define TAM_INICIAL 911 
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
 * 'Baja' el elemento las posiciones necesarias hasta cunmplir la propiedad de heap
 * de maximo, a traves del arreglo pasado por parametro
 */
void down_heap(void* arreglo[], size_t tam, cmp_func_t cmp, size_t pos);

/*
 * 'Sube' el elemento las posiciones necesarias hasta cunmplir la propiedad de heap
 * de maximo, a traves del arreglo pasado por parametro
 */
void up_heap(void* arreglo[], size_t tam, cmp_func_t cmp, size_t pos);

/*
 * Pre: Recibe un arreglo ya creado
 * Post: El arreglo ahora cumple con las propiedades del heap
 */
void heapify(void* arreglo[], size_t tam, cmp_func_t cmp);


/*
 * Pre: Recibe un arreglo creado
 * Post: El arreglo ahora esta ordenado de menor a mayor
 */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp);

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
        swap(&arreglo[pos],&arreglo[h_izq]);
    }
    else if(h_der < tam) {

        // Comparo y devuelvo la pos del mayor entre los dos hijos
        size_t hijo_mayor = (cmp(arreglo[h_izq], arreglo[h_der]) < 0) ? h_der : h_izq;

        // Comparo si el padre es mayor que el hijo mayor
        if(cmp(arreglo[pos], arreglo[hijo_mayor]) < 0) {
            swap(&arreglo[pos], &arreglo[hijo_mayor]);
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
    for(size_t i = tam ; i > 0; i--) {
        down_heap(arreglo, tam, cmp, i - 1);
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

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heapify(elementos, cant, cmp);

    while (cant > 1) {
        swap(&elementos[0], &elementos[cant - 1]);
        cant--;
        down_heap(elementos, cant, cmp, 0);
    }
}

/* ******************************************************************
 *                      PRIMITIVAS DEL HEAP
 * *****************************************************************/


heap_t *heap_crear(cmp_func_t cmp) {
    if(!cmp) return NULL;

    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;

    void** datos = malloc(sizeof(void*) * TAM_INICIAL);
    if(!datos){
        free(heap);
        return NULL;
    }

    heap->datos = datos;
    heap->cantidad = 0;
    heap->capacidad = TAM_INICIAL;
    heap->cmp = cmp;

    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    if(!cmp) return NULL;

    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;

    heap->datos = malloc(sizeof(void*) * n);
    if(!heap->datos) {
        free(heap);
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        /*heap->datos[i] = malloc(sizeof(void*));
        if(!heap->datos[i]){
            for(int j = 0; j < i; j++){
                free(heap->datos[j]);
            }
            free(heap->datos);
            free(heap);
            return NULL;
        }
        memcpy(heap->datos[i], arreglo[i], sizeof(void*));*/
        heap->datos[i] = arreglo[i];
	}

    heapify(heap->datos, n, cmp);


    heap->capacidad = n ;
    heap->cantidad = n;
    heap->cmp = cmp;

    return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {
    for(size_t i = 0; i < heap->cantidad; i++) {
        if(destruir_elemento != NULL) {
            destruir_elemento(heap->datos[i]);
        } /*else {
            free(heap->datos[i]);
        }*/
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

    /*heap->datos[heap->cantidad] = malloc(sizeof(void*));
    if(!heap->datos[heap->cantidad]) return false;*/
    heap->datos[heap->cantidad] = elem;

    /*memcpy(heap->datos[heap->cantidad], elem, sizeof(void*));*/
    up_heap(heap->datos, heap->cantidad, heap->cmp, heap->cantidad);
    heap->cantidad++;

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


    void* dato = heap_ver_max(heap);

    //Intercambio primero y ultimo
    swap(&heap->datos[0], &heap->datos[heap->cantidad - 1]);

    /*free(heap->datos[heap->cantidad-1]);*/

    // Disminuyo en 1 la cantidad
    heap->cantidad--;

    //Redimension
    if (heap->cantidad < (heap->capacidad / MULTIPLO_CANTIDAD)) {
        heap_redimensionar(heap, heap->capacidad / FACTOR_REDIMENSION);
    }

    down_heap(heap->datos, heap->cantidad, heap->cmp, 0);
    
    return dato;
}