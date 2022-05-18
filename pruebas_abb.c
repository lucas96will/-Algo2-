#include <stdio.h>
#include <string.h>
#include "testing.h"
#include "abb.h"
#include <time.h>
#include <stdlib.h>

#define TAM_VOLUMEN 5000
#define TAM_CLAVE 5
#define TAM_NORMAL 100



static void prueba_abb_vacio() {
    
    abb_t* abb = abb_crear(NULL, NULL);

    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

static void prueba_abb_ejemplo_basico(){

    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau", *valor5 = "guauguau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu", *valor4 = "muuu";
    
    
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));

    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));

    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb la cantidad de elementos es 3", abb_cantidad(abb) == 3);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));

    print_test("Prueba abb insertar clave3 con valor 4", abb_guardar(abb, clave3, valor4));
    print_test("Prueba abb la cantidad de elementos es igual que antes", abb_cantidad(abb) == 3);
    print_test("Prueba abb obtener clave3 es valor4", abb_obtener(abb, clave3) == valor4);

    print_test("Prueba abb insertar clave de la raiz con valor 5", abb_guardar(abb, clave1, valor5));
    print_test("Prueba abb la cantidad de elementos es igual que antes", abb_cantidad(abb) == 3);
    print_test("Prueba abb obtener clave1 es valor5", abb_obtener(abb, clave1) == valor5);
    
    
    print_test("Borro al nodo raiz", abb_borrar(abb, clave1) == valor5);
    print_test("Prueba abb obtener clave1 es NULL", abb_obtener(abb, clave1) == NULL);
    print_test("Prueba abb obtener clave3 es valor4", abb_obtener(abb, clave3) == valor4);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);

    print_test("Borro al nodo con clave 3", abb_borrar(abb, clave3) == valor4);
    print_test("Prueba abb obtener clave3 es NULL", abb_obtener(abb, clave3) == NULL);
    
    print_test("Borro al nodo con clave 2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb obtener clave2 es NULL", abb_obtener(abb, clave2) == NULL);

    
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    

    abb_destruir(abb);
}

static void prueba_abb_reemplazar_con_destruir() {
    abb_t* abb = abb_crear(strcmp, free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    /* Se destruye el abb (se debe liberar lo que quedó dentro) */
    abb_destruir(abb);
}
 
static void prueba_abb_clave_vacia()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = "";

    print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    abb_destruir(abb);
}

static void prueba_abb_valor_null()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void pruebas_abb_borrar() {
    abb_t* abb = abb_crear(strcmp, NULL);
    /*Arbol: 
                        gato                                
            foca                        leon               
        buho    gallo           gorila        lobo
                                           loba
    */
    char *c_gato = "gato", *v_gato = "miau";
    char *c_foca = "foca", *v_foca = "focaaa";
    char *c_leon = "leon", *v_leon = "roar";
    char *c_buho = "buho", *v_buho = "buho";
    char *c_gallo = "gallo", *v_gallo = "poporopo";
    char *c_gorila = "gorila", *v_gorila = "pum";
    char *c_lobo = "lobo", *v_lobo = "auuuu";
    char *c_loba = "loba", *v_loba = "auiaaa";

    print_test("Prueba abb inserto todo", abb_guardar(abb, c_gato, v_gato));
    print_test("Prueba abb inserto todo", abb_guardar(abb, c_foca, v_foca));
    print_test("Prueba abb inserto todo", abb_guardar(abb, c_leon, v_leon));
    print_test("Prueba abb inserto todo", abb_guardar(abb, c_buho, v_buho));
    print_test("Prueba abb inserto todo", abb_guardar(abb, c_gallo, v_gallo));
    print_test("Prueba abb inserto todo", abb_guardar(abb, c_gorila, v_gorila));
    print_test("Prueba abb inserto todo", abb_guardar(abb, c_lobo, v_lobo));
    print_test("Prueba abb inserto todo", abb_guardar(abb, c_loba, v_loba));
    print_test("La cantidad es 4", abb_cantidad(abb) == 8);
    
    print_test("Borrar elemento con 2 hijos", abb_borrar(abb, c_leon) == v_leon);
    print_test("Pruebo si esta clave de esa zona", abb_pertenece(abb, c_loba));
    /*Arbol: 
                        gato                                
            foca                        loba               
        buho    gallo           gorila        lobo
                                           
    */

    print_test("Borrar elemento con 0 hijos", abb_borrar(abb, c_gorila) == v_gorila);
    print_test("Pruebo si esta clave de esa zona", abb_pertenece(abb, c_lobo));
    /*Arbol: 
                        gato                                
            foca                        loba               
        buho    gallo                       lobo
                                           
    */

    print_test("Borrar elemento con 1 hijo", abb_borrar(abb, c_loba) == v_loba);
    print_test("Pruebo si esta clave de esa zona", abb_pertenece(abb, c_lobo));

    /*Arbol: 
                        gato                                
            foca                        lobo               
        buho    gallo                       
                                           
    */
    print_test("Borrar raiz", abb_borrar(abb, c_gato) == v_gato);
    print_test("Pruebo si esta clave de esa zona", abb_pertenece(abb, c_gallo));
    /*Arbol: 
                        lobo                                
            foca                                       
        buho    gallo                       
                                           
    */
   print_test("La cantidad es 4", abb_cantidad(abb) == 4);

   abb_destruir(abb);

}

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}


static void pruebas_iter_in() {

    abb_t* arbol = abb_crear(strcmp, NULL);
    char** claves = malloc(sizeof(char*) * TAM_NORMAL);
    if(!claves) {
        return;
    }


    for(int i = 0; i < TAM_NORMAL; i++) {
        char* clave = malloc(sizeof(char)*TAM_CLAVE);
        claves[i] = rand_string(clave, TAM_CLAVE);
        abb_guardar(arbol, claves[i], claves[i]);
    }

    // Pruebas genericas del iterador
    abb_iter_t* iter = abb_iter_in_crear(arbol);
    print_test("Iterador inorder creado", iter);
    print_test("Avanzar iterador es posible", abb_iter_in_avanzar(iter));
    print_test("Obtener iterador no es null", abb_iter_in_ver_actual(iter));
    print_test("Iterador no se encuentra al final", !abb_iter_in_al_final(iter));

    const char* clave_x = abb_iter_in_ver_actual(iter);
    // La clave del iterador se encuentra en el arbol
    print_test("Clave X del iterador se encuentra en el arbol", abb_pertenece(arbol,clave_x));
    abb_iter_in_avanzar(iter);
    const char* clave_y = abb_iter_in_ver_actual(iter);
    print_test("Avanzo y la clave X del iterador se encuentra en el arbol", abb_pertenece(arbol,clave_x));
    print_test("La clave del iterador es diferente a la anterior", strcmp(clave_x, clave_y) != 0);

    bool avanzar = true;
    while(!abb_iter_in_al_final(iter)) {
        avanzar &= abb_iter_in_avanzar(iter);
    }
    print_test("Se pudo avanzar hasta el final correctamente", avanzar);
    print_test("Iterador al final", abb_iter_in_al_final(iter));
    print_test("No se puede avanzar el iterador", !abb_iter_in_avanzar(iter));
    print_test("No se puede obtener clave del iterador", !abb_iter_in_ver_actual(iter));

    for(int i = 0; i < TAM_NORMAL; i++) {
        free(claves[i]);
    }
    free(claves);
    abb_iter_in_destruir(iter);
    abb_destruir(arbol);

}

static void pruebas_volumen(const size_t tam) {
    abb_t* arbol = abb_crear(strcmp, NULL);

    char** claves = malloc(sizeof(char*)*tam);
    if(!claves) {
        return;
    }

    bool guardar_ok = true;
    for(int i = 0; i < tam; i++) {
        char* clave = malloc(sizeof(char)*TAM_CLAVE);
        claves[i] = rand_string(clave, TAM_CLAVE);
        guardar_ok &= abb_guardar(arbol, claves[i], claves[i]);
    }

    print_test("Se pudo guardar gran cantidad de datos correctamente", guardar_ok);

    bool obtener_ok = true;
    for(int i = 0; i < tam; i++) {
        char* actual = abb_obtener(arbol, claves[i]);
        obtener_ok &= (strcmp(actual, claves[i]) == 0);
    }

    print_test("Se pudo obtener todos los elementos correctamente", obtener_ok);

    for(int i = 0; abb_cantidad(arbol) > 0; i++) {
        // Puede haber claves repetidas, por lo que a veces el borrar puede devolver null!
        abb_borrar(arbol, claves[i]);
    }
    print_test("Se pudo borrar todos los elementos correctamente", abb_cantidad(arbol) == 0);


    for(int i = 0; i < tam; i++) {
        free(claves[i]);
    }
    free(claves);
    abb_destruir(arbol);
}

static void pruebas_iterador_volumen(const size_t tam) {
    // Creo arbol y guardo elementos

    abb_t* arbol = abb_crear(strcmp, NULL);
    char** claves = malloc(sizeof(char*)*tam);
    if(!claves) return;

    for(int i = 0; i < tam; i++) {
        char* clave = malloc(sizeof(char)*TAM_CLAVE);
        claves[i] = rand_string(clave, TAM_CLAVE);
        abb_guardar(arbol, claves[i], claves[i]);
    }

    abb_iter_t* iter = abb_iter_in_crear(arbol);
    if(!iter){
        abb_destruir(arbol);
        return;
    }

    char** claves_v2 = malloc(sizeof(char*) *abb_cantidad(arbol));
    if(!claves_v2) {
        for(int i = 0; i < abb_cantidad(arbol); i++) {
            free(claves[i]);
        }
        return;
    }

    // Itero mientras voy guardando en un vector en mi vector de claves

    for(int i = 0; !abb_iter_in_al_final(iter); i++) {
        const char* actual = abb_iter_in_ver_actual(iter);
        claves_v2[i] = malloc(sizeof(char)*TAM_CLAVE);
        strcpy(claves_v2[i], actual);
        abb_iter_in_avanzar(iter);
    }

    bool ordenado = true;
    for(int i = 0; i < abb_cantidad(arbol)-1 && ordenado; i++) {
        ordenado &= (strcmp(claves_v2[i], claves_v2[i+1]) < 0);
    }

    print_test("El iterador inorder itero correctamente sobre toda la estructura", ordenado);


    // Libero toda la memoria pedida

    for(int i = 0; i < tam; i++) {
        free(claves[i]);
    }
    for(int i = 0; i < abb_cantidad(arbol); i++) {
        free(claves_v2[i]);
    }

    free(claves);
    free(claves_v2);
    abb_iter_in_destruir(iter);
    abb_destruir(arbol);
}



void pruebas_abb_estudiante() {
    srand((unsigned)time(NULL));
    prueba_abb_vacio();
    prueba_abb_ejemplo_basico();
    prueba_abb_reemplazar_con_destruir();
    prueba_abb_valor_null();
    prueba_abb_clave_vacia();
    pruebas_abb_borrar();
    pruebas_iter_in();
    pruebas_volumen(TAM_VOLUMEN);
    pruebas_iterador_volumen(TAM_VOLUMEN);
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_abb_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif