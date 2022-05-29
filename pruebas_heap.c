#include "heap.h"
#include "testing.h"
#include <stdio.h>

#define TAM_PRUEBA 10
#define TAM_VOLUMEN 5000


/* ******************************************************************
 *            FUNCIONES AUXILIARES PARA PRUEBAS
 * *****************************************************************/

int comparar_int(const void* a, const void* b) {
    if(*(int*)a < *(int*)b) {
        return -1;
    } else if(*(int*)a > *(int*)b) {
        return 1;
    }
    return 0;
}


/* ******************************************************************
 *                      PRUEBAS HEAP
 * *****************************************************************/

static void prueba_heap_crear() {
    printf("\nINICIO DE PRUEBA HEAP CREAR\n");

    heap_t* heap = heap_crear(comparar_int);

    print_test("El heap fue creado", heap != NULL);
    print_test("El heap no tiene cantidad 0", heap_cantidad(heap) == 0);
    print_test("El heap esta vacio", heap_esta_vacio(heap));
    print_test("Ver max en heap vacio es NULL", heap_ver_max(heap) == NULL);
    print_test("Desencoplar en heap vacio es NULL", heap_desencolar(heap) == NULL);

    heap_destruir(heap, NULL);

    /*void** vec = malloc(sizeof(void*) * TAM_PRUEBA);
    for(int i = 0; i < TAM_PRUEBA; i++){
        vec[i] = malloc(sizeof(int));
        *(int*)vec[i] = i+1;
    }*/
    int vec2[TAM_PRUEBA] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    void** vec = malloc(sizeof(void*) * TAM_PRUEBA);
    for(int i = 0; i < TAM_PRUEBA; i++) {
        vec[i] = &vec2[i];
    }
    heap_t* heap_2 = heap_crear_arr(vec, TAM_PRUEBA, comparar_int);
    print_test("Heap crear arr fue creado exitosamente", heap_2 != NULL);
    print_test("El heap tiene 10 elementos", heap_cantidad(heap_2) == 10);
    print_test("El heap no esta vacio", heap_esta_vacio(heap_2) == false);
    print_test("Ver max del heap es 10", *(int*)heap_ver_max(heap_2) == 10);



    free(vec);
    heap_destruir(heap_2, NULL);
}

static void prueba_heap_encolar() {
    printf("\nINICIO DE PRUEBA HEAP ENCOLAR\n");

    heap_t* heap = heap_crear(comparar_int);

    int vec[TAM_PRUEBA] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    bool ok = true;
    for(int i = 0; i < TAM_PRUEBA; i++) {
        ok &= heap_encolar(heap, &vec[i]);
        ok &= (*(int*)heap_ver_max(heap) == i + 1);
    }
    print_test("Ver max del heap es 10", *(int*)heap_ver_max(heap) == 10);

    print_test("Se pudo encolar un arreglo del 1 al 10 correctamente", ok);


    heap_destruir(heap, NULL);

}

static void prueba_heap_desencolar() {
    printf("\nINICIO DE PRUEBA HEAP DESENCOLAR\n");

    heap_t* heap = heap_crear(comparar_int);

    int vec[TAM_PRUEBA] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for(int i = 0; i < TAM_PRUEBA; i++) {
        heap_encolar(heap, &vec[i]);
    }

    bool ok = true;
    for(int i = 10; !heap_esta_vacio(heap); i--) {
        void* vector[TAM_PRUEBA];
        vector[i] = heap_desencolar(heap);
        ok &= (*(int*)vector[i] == i);
        free(vector[i]);
    }

    print_test("Se desencolan varios numeros correctamente", ok);

    heap_destruir(heap, NULL);

}


void pruebas_heap_estudiante() {
    prueba_heap_crear();
    prueba_heap_encolar();
    prueba_heap_desencolar();
//    prueba_heap_esta_vacia();
//    prueba_heap_cantidad();
//    prueba_heap_ver_max();
//    prueba_heap_NULL();
//    prueba_heap_volumen();
//    prueba_heap_sort();
//    prueba_heap_vacio_destruccion_con_free();
//    prueba_heap_de_pilas_con_destruccion();
//    prueba_heap_de_pilas_con_destruccion_manual();

}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_heap_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif