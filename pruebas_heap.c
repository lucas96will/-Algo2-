#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#define TAM_PRUEBA 10
#define TAM_VOLUMEN 10000
#define TAM_ARREGLO 100


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


//funcion aux que mezcla un arreglo (prueba volumen)
void mezclar_arreglo(int arreglo[], size_t tam ) {
    
    for (int i = 0; i < tam; i++) {
        size_t random = rand() % tam;
        int temporal = arreglo[i];
        arreglo[i] = arreglo[random];
        arreglo[random] = temporal;
    }
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
}

static void prueba_heap_primitivas() {
    printf("\nINICIO DE PRUEBA HEAP PRIMITIVAS \n");
    

    heap_t* heap = heap_crear(comparar_int);

    int n1 = 1, n2 = 2, n3 = 5, n4 = 10, n5 = 8, n6 = 6;

    print_test("El heap esta vacio", heap_esta_vacio(heap));
    print_test("Encolo al elemento n1", heap_encolar(heap, &n1));
    print_test("El heap no esta vacio", !heap_esta_vacio(heap));
    print_test("Maximo es el correcto", *(int*)heap_ver_max(heap) == n1);
    print_test("La cantidad del heap es 1", heap_cantidad(heap) == 1);
    print_test("Encolo al elemento n2", heap_encolar(heap, &n2));
    print_test("La cantidad del heap es 2", heap_cantidad(heap) == 2);
    print_test("Maximo es el correcto", *(int*)heap_ver_max(heap) == n2);
    print_test("Encolo al elemento n3", heap_encolar(heap, &n3));
    print_test("La cantidad del heap es 3", heap_cantidad(heap) == 3);
    print_test("Maximo es el correcto", *(int*)heap_ver_max(heap) == n3);
    print_test("Encolo al elemento n4", heap_encolar(heap, &n4));
    print_test("La cantidad del heap es 4", heap_cantidad(heap) == 4);
    print_test("Maximo es el correcto", *(int*)heap_ver_max(heap) == n4);
    print_test("Encolo al elemento n5", heap_encolar(heap, &n5));
    print_test("La cantidad del heap es 5", heap_cantidad(heap) == 5);
    print_test("Maximo es el correcto", *(int*)heap_ver_max(heap) == n4);
    print_test("Encolo al elemento n6", heap_encolar(heap, &n6));
    print_test("La cantidad del heap es 6", heap_cantidad(heap) == 6);
    print_test("Maximo es el correcto", *(int*)heap_ver_max(heap) == n4);

    print_test("Desencolar devuelve al mayor elemento", *(int*)heap_desencolar(heap) == n4);
    print_test("La cantidad del heap es 5", heap_cantidad(heap) == 5);
    print_test("Maximo es el correcto", *(int*)heap_ver_max(heap) == n5);
    print_test("Desencolar devuelve al mayor elemento", *(int*)heap_desencolar(heap) == n5);
    print_test("La cantidad del heap es 4", heap_cantidad(heap) == 4);
    print_test("Desencolar devuelve al mayor elemento", *(int*)heap_desencolar(heap) == n6);
    print_test("La cantidad del heap es 3", heap_cantidad(heap) == 3);


    //Destruyo con algunos elementos
    heap_destruir(heap, NULL);

}

static void pruebas_heap_destruccion_automatica() {
    printf("\nINICIO DE PRUEBA HEAP CON DESTRUCCION AUTOMATICA \n");
    
    heap_t* heap = heap_crear(comparar_int);

    int* n1 = malloc(sizeof(int));
    int* n2 = malloc(sizeof(int));
    int* n3 = malloc(sizeof(int));
    int* n4 = malloc(sizeof(int));
    int* n5 = malloc(sizeof(int));
    int* n6 = malloc(sizeof(int));
    *n1 = 1, *n2 = 2, *n3 = 5, *n4 = 10, *n5 = 8, *n6 = 6;

    print_test("El heap esta vacio", heap_esta_vacio(heap));
    print_test("Encolo al elemento n1", heap_encolar(heap, n1));
    print_test("Encolo al elemento n2", heap_encolar(heap, n2));
    print_test("Encolo al elemento n3", heap_encolar(heap, n3));
    print_test("Encolo al elemento n4", heap_encolar(heap, n4));
    print_test("Encolo al elemento n5", heap_encolar(heap, n5));
    print_test("Encolo al elemento n6", heap_encolar(heap, n6));

    heap_destruir(heap, free);

}

static void pruebas_heap_destruccion_manual() {
    printf("\nINICIO DE PRUEBA HEAP CON DESTRUCCION MANUAL \n");
    heap_t* heap = heap_crear(comparar_int);
    int* n1 = malloc(sizeof(int));
    int* n2 = malloc(sizeof(int));
    int* n3 = malloc(sizeof(int));
    int* n4 = malloc(sizeof(int));
    int* n5 = malloc(sizeof(int));
    int* n6 = malloc(sizeof(int));
    *n1 = 1, *n2 = 2, *n3 = 5, *n4 = 10, *n5 = 8, *n6 = 6;

    print_test("El heap esta vacio", heap_esta_vacio(heap));
    print_test("Encolo al elemento n1", heap_encolar(heap, n1));
    print_test("Encolo al elemento n2", heap_encolar(heap, n2));
    print_test("Encolo al elemento n3", heap_encolar(heap, n3));
    print_test("Encolo al elemento n4", heap_encolar(heap, n4));
    print_test("Encolo al elemento n5", heap_encolar(heap, n5));
    print_test("Encolo al elemento n6", heap_encolar(heap, n6));


    heap_destruir(heap, NULL);
    
    //Libero manualmente la memoria de los elementos
    free(n1);
    free(n2);
    free(n3);
    free(n4);
    free(n5);
    free(n6);
}

static void prueba_volumen(size_t tam) {
    printf("\nINICIO DE PRUEBA HEAP VOLUMEN \n");
    
    heap_t* heap = heap_crear(comparar_int);

    int datos[tam];

    for (int i = 0; i < tam; i++) {
	datos[i] = i;
    }
    int max = datos[tam - 1];
    mezclar_arreglo(datos, tam);

    bool ok_encolar = true;
    for (size_t i = 0; i < tam; i++){
        ok_encolar = heap_encolar(heap, &datos[i]);
        if (!ok_encolar) break;
    }
    print_test("Todos los elementos fueron encolados", ok_encolar);
    print_test("El maximo es correcto", *(int*)heap_ver_max(heap) == max);
    print_test("Cantidad es correcta", heap_cantidad(heap) == tam);

    bool ok_desencolar = true;
    for (size_t i = 0; i < tam; i++) {
        ok_desencolar = (*(int*)heap_desencolar(heap) == max);
        if (!ok_desencolar) break;
    
        max--;
    }
    print_test("Todos los elementos fueron desencolados", ok_desencolar);

    heap_destruir(heap, NULL);
}

static void prueba_heap_desde_arreglo(size_t tam) {
    
    printf("\nINICIO DE PRUEBA HEAP ARREGLO \n");
    

    int numeros[tam];
    void* datos[tam];

    for (int i = 0; i < tam; i++) {
	numeros[i] = i;
    }
    int max = numeros[tam - 1];
    mezclar_arreglo(numeros, tam);

    for (int i = 0; i < tam; i++) {
	datos[i] = &numeros[i];
    }

    heap_t* heap = heap_crear_arr(datos, tam, comparar_int);

    bool ok_encolar = true;
    for (size_t i = 0; i < tam; i++){
        ok_encolar = heap_encolar(heap, datos[i]);
        if (!ok_encolar) break;
    }
    print_test("Todos los elementos fueron encolados", ok_encolar);
    print_test("El maximo es correcto", *(int*)heap_ver_max(heap) == 99);


    bool ok_desencolar = true;
    for (size_t i = 0; i < tam; i++) {
        ok_desencolar &= (*(int*)heap_desencolar(heap) == max);
        ok_desencolar &= (*(int*)heap_desencolar(heap) == max);
        if (!ok_desencolar) break;
        max--;
    }
    print_test("Todos los elementos fueron desencolados", ok_desencolar);

    heap_destruir(heap, NULL);
}

static void prueba_heapsort(size_t tam) {
    printf("\nINICIO DE PRUEBA HEAP SORT \n");

    int numeros[tam];
    void* datos[tam];

	for (int i = 0; i < tam; i++) {
		numeros[i] = i;
	}
    mezclar_arreglo(numeros, tam);

    for (int i = 0; i < tam; i++) {
        datos[i] = &numeros[i];
    }

    heap_sort(datos, tam, comparar_int);

    bool ok_pos = true;
    for (size_t pos = 0; pos < tam; pos++){
        ok_pos &= (*(int*)datos[pos] == pos);
        if (!ok_pos) break;
    }

    print_test("Estan ordenados", ok_pos);
}

void pruebas_heap_estudiante() {
    srand((unsigned int)time(NULL));
    prueba_heap_crear();
    prueba_heap_primitivas();
    pruebas_heap_destruccion_automatica();
    pruebas_heap_destruccion_manual();
    prueba_volumen(TAM_VOLUMEN);
    prueba_heap_desde_arreglo(TAM_ARREGLO);
    prueba_heapsort(TAM_ARREGLO);

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
