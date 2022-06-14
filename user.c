#include "user.h"

#define MAYOR_AFINIDAD_P1 1
#define MAYOR_AFINIDAD_P2 -1

#define MENOR_ID_P1 1
#define MENOR_ID_P2 -1

struct user {
    char* nombre;
    size_t id;
    heap_t* feed;
};


//Para el heap del feed de cada usuario
int comparar_afinidad(const void* publicacion_user_1, const void* publicacion_user_2) {
    publicacion_user_t* p1 = (publicacion_user_t*) publicacion_user_1;
    publicacion_user_t* p2 = (publicacion_user_t*) publicacion_user_2;

    if(publicacion_user_obtener_afinidad(p1) < publicacion_user_obtener_afinidad(p2)) {
        return MAYOR_AFINIDAD_P1;
    } else if(publicacion_user_obtener_afinidad(p1) > publicacion_user_obtener_afinidad(p2)) {
        return MAYOR_AFINIDAD_P2;
    }
    size_t id_p1 = publicacion_obtener_id(publicacion_user_obtener_publicacion(p1));
    size_t id_p2 = publicacion_obtener_id(publicacion_user_obtener_publicacion(p2));
    // Si ambas publicaciones_user tienen la misma afinidad, se ordena por id
    if(id_p1 < id_p2){
        return MENOR_ID_P1;
    }
    else{
        return MENOR_ID_P2;
    }
}

/* *****************************************************************
 *                    PRIMITIVAS USER
 * *****************************************************************/

user_t* user_crear(char* nombre, size_t id) {

    user_t* user = malloc(sizeof(user_t));
    if(!user) {
        return NULL;
    }
    char* nombre_copiado = strdup(nombre);
    user->nombre = nombre_copiado;
    user->id = id;
    user->feed = heap_crear(comparar_afinidad);

    return user;
}

void user_actualizar_feed(user_t* user, publicacion_user_t* publicacion_user) {
    heap_encolar(user->feed, publicacion_user);
}

publicacion_user_t* user_obtener_siguiente_feed(user_t* user){
    return heap_desencolar(user->feed);
}

bool user_feed_vacio(user_t* user) {
    return heap_esta_vacio(user->feed);
}

void _publicacion_user_destruir(void* publicacion_user){
    publicacion_user_destruir((publicacion_user_t*) publicacion_user);
}

char* user_obtener_nombre(user_t* user) {
    return user->nombre;
}

size_t user_obtener_id(user_t* user) {
    return user->id;
}

heap_t* user_obtener_feed(user_t* user) {
    return user->feed;
}


void user_destruir(user_t* user) {
    heap_destruir(user->feed, _publicacion_user_destruir);
    free(user->nombre);
    free(user);
}