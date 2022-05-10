#ifndef _ALGO2__ABB_H
#define _ALGO2__ABB_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

/* *****************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

/*
 * Pre:
 * Post:
 * */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/*
 * Pre:
 * Post:
 * */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/*
 * Pre:
 * Post:
 * */
void *abb_borrar(abb_t *arbol, const char *clave);

/*
 * Pre:
 * Post:
 * */
void *abb_obtener(const abb_t *arbol, const char *clave);

/*
 * Pre:
 * Post:
 * */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/*
 * Pre:
 * Post:
 * */
size_t abb_cantidad(const abb_t *arbol);

/*
 * Pre:
 * Post:
 * */
void abb_destruir(abb_t *arbol);


/* *****************************************************************
 *                    ITER INTERNO DEL ABB
 * *****************************************************************/
/*
 * Pre:
 * Post:
 * */
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);


#endif //_ALGO2__ABB_H