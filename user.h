#ifndef _ALGO2__USER_H
#define _ALGO2__USER_H
#define _POSIX_C_SOURCE 200809L
#include "heap.h"

typedef struct user user_t;

/* *****************************************************************
 *                    PRIMITIVAS USER
 * *****************************************************************/

/*
 * Pre: Recibe un nombre, un id y un heap de feed
 * Post: Devuelve la estructura user creada
 */
user_t* user_crear(char* nombre, size_t id);

/*
 * Pre : user creado
 * Post: Obtiene el nombre del user
 */
char* user_obtener_nombre(user_t* user);

/*
 * Pre : user creado
 * Post: Obtiene el id del user
 */
size_t user_obtener_id(user_t* user);

/*
 * Pre: user_actual creado, publicacion creada
 * Post: Actualiza el feed del usuario con la nueva publicacion
 */
void user_actualizar_feed(user_t* user_actual, void* publicacion_v, size_t cant_users);

/*
 * Pre: User creado
 * Post: devuelve true si el feed se encuentra vacio, false en otro caso
 */
bool user_feed_vacio(user_t* user);

/*
 * Pre: User creado
 * Post: Devuelve la publicacion_user con mas afinidad al user
 */
void* user_obtener_siguiente_feed(user_t* user);

/*
 * Pre: La estructura user fue creada
 * Post: La estructura user fue destruida
 */
void user_destruir(user_t* user);


#endif //_ALGO2__USER_H