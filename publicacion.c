#define _POSIX_C_SOURCE 200809L
#include "publicacion.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct publicacion{
    user_t* user;
    char* mensaje;
    size_t id;
    abb_t* likes;
};

/* *****************************************************************
 *                    PRIMITIVAS PUBLICACION
 * *****************************************************************/

publicacion_t* publicacion_crear(user_t* user, char* mensaje, size_t id) {

    publicacion_t* publicacion = malloc(sizeof(publicacion_t));
    if (!publicacion) {
        return NULL;
    }
    char* mensaje_copiado = strdup(mensaje);
    if(mensaje_copiado == NULL) {
        free(publicacion);
        return NULL;
    }
    publicacion->user = user;
    publicacion->mensaje = mensaje_copiado;
    publicacion->id = id;
    publicacion->likes = abb_crear(strcmp, NULL);

    return publicacion;
}

user_t* publicacion_obtener_user(publicacion_t* publicacion) {
    return publicacion->user;
}

size_t publicacion_obtener_id(publicacion_t*publicacion) {
    return publicacion->id;
}

abb_t* publicacion_obtener_likes(publicacion_t* publicacion) {
    return publicacion->likes;
}

bool publicacion_agregar_like(publicacion_t* publicacion, user_t* usuario) {
    return abb_guardar(publicacion->likes, user_obtener_nombre(usuario), usuario);
}

size_t publicacion_cantidad_likes(publicacion_t* publicacion) {
    return abb_cantidad(publicacion->likes);
}

void publicacion_imprimir(publicacion_t* publicacion) {
    fprintf(stdout, "Post ID %zu\n", publicacion->id);
    fprintf(stdout, "%s dijo: ", user_obtener_nombre(publicacion->user));
    fprintf(stdout, "%s\n", publicacion->mensaje);
    fprintf(stdout, "Likes: %zu\n", abb_cantidad(publicacion->likes));
}

void publicacion_destruir(publicacion_t* publicacion) {
    abb_destruir(publicacion->likes);
    free(publicacion->mensaje);
    free(publicacion);
}