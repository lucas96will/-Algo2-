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
 * Pre:
 * Post:
 */
user_t* user_crear(char* nombre, size_t id, heap_t* feed);

/*
 * Pre:
 * Post:
 */
void user_actualizar_feed(user_t* user, void* publicacion_user);

/*
 * Pre:
 * Post:
 */
void user_destruir(user_t* user);


#endif //_ALGO2__USER_H