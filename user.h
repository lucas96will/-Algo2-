#ifndef _ALGO2__USER_H
#define _ALGO2__USER_H
#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>
#include "heap.h"

typedef struct user {
    char* nombre;
    size_t id;
    heap_t* feed;
}user_t;

/* *****************************************************************
 *                    PRIMITIVAS USER
 * *****************************************************************/

/*
 * Pre: Recibe un nombre, un id y un heap de feed
 * Post: Devuelve la estructura user creada
 */
user_t* user_crear(char* nombre, size_t id, heap_t* feed);

/*
 * Pre: Recibe un usuario y una publicacion_user
 * Post: Actualiza el feed del usuario con la nueva publicacion_user
 */
void user_actualizar_feed(user_t* user, void* publicacion_user);

/*
 * Pre: La estructura user fue creada
 * Post: La estructura user fue destruida
 */
void user_destruir(user_t* user);


#endif //_ALGO2__USER_H