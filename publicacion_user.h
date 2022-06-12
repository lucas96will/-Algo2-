#ifndef _ALGO2__PUBLICACION_USER_H
#define _ALGO2__PUBLICACION_USER_H

#include "publicacion.h"

typedef struct publicacion_user {
    publicacion_t* publicacion;
    size_t afinidad;
} publicacion_user_t;


/* *****************************************************************
 *                    PRIMITIVAS PUBLICACION_USER
 * *****************************************************************/

/*
 * Pre: Recibe una publicacion y el numero de afinidad
 * Post: La estructura publicacion_user fue creada
 */
publicacion_user_t* publicacion_user_crear(publicacion_t* publicacion, size_t afinidad);

/*
 * Pre: La estructura publicacion_user fue creada
 * Post: La estructura publicacion_user fue destruida
 */
void publicacion_user_destruir(publicacion_user_t* publicacion_user);

#endif //_ALGO2__PUBLICACION_USER_H