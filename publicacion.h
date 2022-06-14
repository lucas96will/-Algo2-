#ifndef _ALGO2__PUBLICACION_H
#define _ALGO2__PUBLICACION_H

#include "abb.h"
#include "user.h"

typedef struct publicacion publicacion_t;
typedef struct user user_t;

/* *****************************************************************
 *                    PRIMITIVAS USER
 * *****************************************************************/

/*
 * Pre: Recibe un user, un mensaje, un id y un abb
 * Post: Devuelve la estrcutura publicacion creada
 */
publicacion_t* publicacion_crear(user_t* user, char* mensaje, size_t id);


/*
 * Pre : Publicacion creada
 * Post: Obtiene el user de la publicacion
 */
user_t* publicacion_obtener_user(publicacion_t* publicacion);

/*
 * Pre : Publicacion creada
 * Post: Obtiene el user de la publicacion
 */
char* publicacion_obtener_mensaje(publicacion_t* publicacion);

/*
 * Pre : Publicacion creada
 * Post: Obtiene el id de la publicacion
 */
size_t publicacion_obtener_id(publicacion_t* publicacion);

/*
 * Pre: Publicacion creada
 * Post: Agrega el nombre del usuario y el usuario en los likes de la publicacion
 */
bool publicacion_agregar_like(publicacion_t* publicacion, user_t* user);

/*
 * Pre: Publicacion creada
 * Post: Obtiene el numero de likes de la publicacion
 */
size_t publicacion_cantidad_likes(publicacion_t* publicacion);

/*
 * Pre : Publicacion creada
 * Post: Obtiene el abb de likes de la publicacion
 */
abb_t* publicacion_obtener_likes(publicacion_t* publicacion);

/*
 * Pre : Publicacion creada
 * Post: Imprime por pantalla la publicacion, ejemplo:
 *      Post ID
 *      user dijo:
 *      Mensaje: <mensaje>
 *      Likes: <likes>
 *
 */
void publicacion_imprimir(publicacion_t* publicacion);

/*
 * Pre: La estructura publicacion fue creada
 * Post: La estructura publuicacion fue destruida
 */
void publicacion_destruir(publicacion_t* publicacion);



#endif //_ALGO2__PUBLICACION_H