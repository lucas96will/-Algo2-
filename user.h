#ifndef _ALGO2__USER_H
#define _ALGO2__USER_H

#include "heap.h"
#include "structs.h"
#include "publicacion_user.h"


typedef struct user user_t;

/* *****************************************************************
 *                    PRIMITIVAS USER
 * *****************************************************************/

user_t* user_crear(char* nombre, size_t id, heap_t* feed);

void user_actualizar_feed(user_t* user, publicacion_user_t* publicacion_user);

void user_destruir(user_t* user);


#endif //_ALGO2__USER_H
