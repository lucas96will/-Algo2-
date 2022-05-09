#include "abb.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct nodo nodo_t;

struct nodo {
    nodo_t* derecha;
    nodo_t* izquierda;
    void* dato;
    void* clave;
};

struct abb {
    nodo_t* raiz;
};



/* *****************************************************************
 *                    FUNCIONES AUXILIARES DEL ABB
 * *****************************************************************/




/* *****************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);


bool abb_guardar(abb_t *arbol, const char *clave, void *dato);


void *abb_borrar(abb_t *arbol, const char *clave);


void *abb_obtener(const abb_t *arbol, const char *clave);


bool abb_pertenece(const abb_t *arbol, const char *clave);


size_t abb_cantidad(const abb_t *arbol);


void abb_destruir(abb_t *arbol);