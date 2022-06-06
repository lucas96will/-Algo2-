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
 * Pre:
 * Post:
 */
publicacion_user_t* publicacion_user_crear(publicacion_t* publicacion, size_t afinidad);

/*
 * Pre:
 * Post:
 */
void publicacion_user_destruir(publicacion_user_t* publicacion_user);

#endif //_ALGO2__PUBLICACION_USER_H
