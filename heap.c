#include "heap.h"

#define RATIO 0.6
#define RESIZE 2

typedef struct heap {
    void** arreglo;
    size_t cant;
    size_t tam;
    cmp_func_t* cmp;
} heap_t;



/* ******************************************************************
 *                FUNCIONES AUXILIARES PARA HEAP
 * *****************************************************************/




/* ******************************************************************
 *                      PRIMITIVAS DEL HEAP
 * *****************************************************************/


heap_t *heap_crear(cmp_func_t cmp) {
    if(!cmp) return NULL;



    return heap;
}
