#include "publicacion_user.h"

struct publicacion_user{
    publicacion_t* publicacion;
    size_t afinidad;
};


/* *****************************************************************
 *                    PRIMITIVAS PUBLICACION_USER
 * *****************************************************************/

publicacion_user_t* publicacion_user_crear(void* publicacion, size_t afinidad) {
    publicacion_user_t* publicacion_user = malloc(sizeof(publicacion_user_t));
    if (!publicacion_user){
        return NULL;
    }

    publicacion_user->publicacion = publicacion;
    publicacion_user->afinidad = afinidad;

    return publicacion_user;
}

publicacion_t* publicacion_user_obtener_publicacion(publicacion_user_t* publicacion_user) {
    return publicacion_user->publicacion;
}

size_t publicacion_user_obtener_afinidad(publicacion_user_t* publicacion_user) {
    return publicacion_user->afinidad;
}

void publicacion_user_destruir(publicacion_user_t* publicacion_user) {
    free(publicacion_user);
}