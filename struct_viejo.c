#include <stdio.h>
#include "heap.h"
#include "lista.h"
#include "hash.h"
#include "abb.h"

/* *****************************************************************
 *                          STRUCT USER
 * *****************************************************************/

typedef struct user {
    char* nombre;
    size_t id;
    heap_t* feed;
} user_t;

user_t* user_crear(char* nombre, size_t id, heap_t* feed) {

    user_t* user = malloc(sizeof(user_t));
    if(!user) {
        return NULL;
    }

    user->nombre = nombre;
    user->id = id;
    user->feed = feed;

    return user;
}

void user_actualizar_feed(user_t* user, publicacion_user_t* publicacion_user) {
    heap_encolar(user->feed, publicacion_user);
}

void user_destruir(user_t* user) {
    heap_destruir(user->feed, NULL);
    free(user);
}

/* *****************************************************************
 *                      STRUCT PUBLICACION
 * *****************************************************************/

typedef struct publicacion {
    user_t* user;
    char* mensaje;
    size_t id;
    abb_t* likes;
} publicacion_t;

publicacion_t* publicacion_crear(user_t* user, char* mensaje, size_t id, abb_t* likes) {

    publicacion_t* publicacion = malloc(sizeof(publicacion_t));
    if (!publicacion) {
        return NULL;
    }

    publicacion->user = user;
    publicacion->mensaje = mensaje;
    publicacion->id = id;
    publicacion->likes = likes;

    return publicacion;
}

void publicacion_destruir(publicacion_t* publicacion) {
    user_destruir(publicacion->user);
    abb_destruir(publicacion->likes);
    free(publicacion);
}

/* *****************************************************************
 *                    STRUCT PUBLICACION USER
 * *****************************************************************/

typedef struct publicacion_user {
    publicacion_t* publicacion;
    size_t afinidad;
} publicacion_user_t;

publicacion_user_t* publicacion_user_crear(publicacion_t* publicacion, size_t afinidad) {
    publicacion_user_t* publicacion_user = malloc(sizeof(publicacion_user_t));
    if (!publicacion_user){
        return NULL;
    }

    publicacion_user->publicacion = publicacion;
    publicacion_user->afinidad = afinidad;
}

publicacion_user_t* publicacion_user_destruir(publicacion_user_t* publicacion_user) {
    publicacion_destruir(publicacion_user->publicacion);
    free(publicacion_user);
}


