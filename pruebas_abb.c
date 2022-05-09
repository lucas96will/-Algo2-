#include <stdio.h>
#include "testing.h"
#include "abb.h"

static void prueba_abb_vacio() {
    //
}




void pruebas_lista_estudiante() {
    //..
    prueba_abb_vacio();

}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif