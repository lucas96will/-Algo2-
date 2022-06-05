#ifndef _ALGO2__STRUCTS_H
#define _ALGO2__STRUCTS_H

#include "abb.h"

typedef struct user {
    char* nombre;
    size_t id;
    heap_t* feed;
} user_t;

typedef struct publicacion {
    user_t* user;
    char* mensaje;
    size_t id;
    abb_t* likes;
} publicacion_t;

typedef struct publicacion_user {
    publicacion_t* publicacion;
    size_t afinidad;
} publicacion_user_t;

#endif //_ALGO2__STRUCTS_H
