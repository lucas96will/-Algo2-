#include "user.h"

/* *****************************************************************
 *                    PRIMITIVAS USER
 * *****************************************************************/

user_t* user_crear(char* nombre, size_t id, heap_t* feed) {

    user_t* user = malloc(sizeof(user_t));
    if(!user) {
        return NULL;
    }
    char* nombre_copiado = strdup(nombre);
    user->nombre = nombre_copiado;
    user->id = id;
    user->feed = feed;

    return user;
}

void user_actualizar_feed(user_t* user, void* publicacion_user) {
    heap_encolar(user->feed, publicacion_user);
}

void user_destruir(user_t* user) {
    heap_destruir(user->feed, NULL);
    free(user->nombre);
    free(user);
}