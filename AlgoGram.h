#ifndef _ALGO2__ALGOGRAM_H
#define _ALGO2__ALGOGRAM_H

#include "user.h"
#include "publicacion.h"
#include "hash.h"
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif // _POSIX_C_SOURCE
#include <stdio.h>

/* *****************************************************************
 *                          FUNCIONES
 * *****************************************************************/

int comparar_afinidad(const void* publicacion_user_1, const void* publicacion_user_2);

FILE* resultado_archivo(int cant_argumentos, char** argumentos);

hash_t* user_a_hash(FILE* archivo);

/* *****************************************************************
 *                          USER LOGIN
 * *****************************************************************/

bool verificaciones_login(hash_t* users, char* user, user_t* user_logeado);

user_t* user_login(hash_t* users, char* user, user_t* user_logeado);

/* *****************************************************************
 *                          USER LOGOUT
 * *****************************************************************/

bool verificaciones_alguien_logeado(user_t* user_logeado);

void user_logout(user_t* user_logeado);


/* *****************************************************************
 *                     PUBLICAR UN POST
 * *****************************************************************/

void publicar_post(user_t* user_logeado, hash_t* users, hash_t* publicaciones, char* mensaje);

void publicacion_a_users(publicacion_t* publicacion, hash_t* users);


/* *****************************************************************
 *                     VER PROXIMO POST
 * *****************************************************************/

bool verificaciones_ver_proximo(user_t* user_logeado);

void ver_proximo_post(user_t* user_logeado);


/* *****************************************************************
 *                     LIKEAR UN POST
 * *****************************************************************/

bool verificaciones_likear_post(user_t* user_logeado, size_t id, hash_t* publicaciones);

void likear_post(user_t* user_logeado, size_t id, hash_t* publicaciones);

/* *****************************************************************
 *                     MOSTRAR LIKES
 * *****************************************************************/

bool verificaciones_ver_likes(user_t* user_logeado, size_t id, hash_t* publicaciones);

void mostrar_likes(user_t* user_logeado, size_t id, hash_t* publicaciones);


#endif //_ALGO2__ALGOGRAM_H
