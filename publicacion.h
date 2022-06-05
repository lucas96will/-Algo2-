#ifndef _ALGO2__PUBLICACION_H
#define _ALGO2__PUBLICACION_H

#include "structs.h"
#include "abb.h"
#include "user.h"

typedef struct publicacion publicacion_t;


/* *****************************************************************
 *                    PRIMITIVAS USER
 * *****************************************************************/

publicacion_t* publicacion_crear(user_t* user, char* mensaje, size_t id, abb_t* likes);

void publicacion_destruir(publicacion_t* publicacion);



#endif //_ALGO2__PUBLICACION_H
