#ifndef _ALGO2__ABB_H
#define _ALGO2__ABB_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

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


/* *****************************************************************
 *                    ITER EXTERNO DEL ABB
 * *****************************************************************/
/*
 * Pre:
 * Post:
 * */
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/*
 * Pre:
 * Post:
 * */
bool abb_iter_in_avanzar(abb_iter_t *iter);

/*
 * Pre:
 * Post:
 * */
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/*
 * Pre:
 * Post:
 * */
bool abb_iter_in_al_final(const abb_iter_t *iter);

/*
 * Pre:
 * Post:
 * */
void abb_iter_in_destruir(abb_iter_t* iter);


#endif //_ALGO2__ABB_H