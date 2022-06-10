#ifndef _ALGO2__PUBLICACION_H
#define _ALGO2__PUBLICACION_H

#include "abb.h"
#include "user.h"

typedef  struct publicacion {
    user_t* user;
    char* mensaje;
    size_t id;
    abb_t* likes;
} publicacion_t;

/* *****************************************************************
 *                    PRIMITIVAS USER
 * *****************************************************************/

/*
 * Pre:
 * Post:
 */
publicacion_t* publicacion_crear(user_t* user, char* mensaje, size_t id, abb_t* likes);

/*
 * Pre:
 * Post:
 */
void publicacion_destruir(publicacion_t* publicacion);



#endif //_ALGO2__PUBLICACION_H