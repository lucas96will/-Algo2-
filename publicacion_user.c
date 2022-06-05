#include "publicacion_user.h"



/* *****************************************************************
 *                    PRIMITIVAS PUBLICACION_USER
 * *****************************************************************/

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