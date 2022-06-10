#include "publicacion.h"
#define _POSIX_C_SOURCE 200809L
#include <string.h>

/* *****************************************************************
 *                    PRIMITIVAS PUBLICACION
 * *****************************************************************/

publicacion_t* publicacion_crear(user_t* user, char* mensaje, size_t id, abb_t* likes) {

    publicacion_t* publicacion = malloc(sizeof(publicacion_t));
    if (!publicacion) {
        return NULL;
    }
    char* mensaje_copiado = strdup(mensaje);
    publicacion->user = user;
    publicacion->mensaje = mensaje_copiado;
    publicacion->id = id;
    publicacion->likes = likes;

    return publicacion;
}

void publicacion_destruir(publicacion_t* publicacion) {
    user_destruir(publicacion->user);
    abb_destruir(publicacion->likes);
    free(publicacion->mensaje);
    free(publicacion);
}