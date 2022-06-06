#ifndef _ALGO2__FUNCIONES_H
#define _ALGO2__FUNCIONES_H

#include "user.h"
#include "publicacion.h"
#include "publicacion_user.h"
#include "hash.h"
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

/* *****************************************************************
 *                          FUNCIONES
 * *****************************************************************/

/*
 * Pre:
 * Post:
 */
int comparar_afinidad(const void* publicacion_user_1, const void* publicacion_user_2);

/*
 * Pre:
 * Post:
 */
FILE* resultado_archivo(int cant_argumentos, char** argumentos);

/*
 * Pre:
 * Post:
 */
hash_t* user_a_hash(FILE* archivo);

/* *****************************************************************
 *                          USER LOGIN
 * *****************************************************************/

/*
 * Pre:
 * Post:
 */
bool verificaciones_login(hash_t* users, char* user, user_t* user_logeado);

/*
 * Pre:
 * Post:
 */
user_t* user_login(hash_t* users, char* user, user_t* user_logeado);

/* *****************************************************************
 *                          USER LOGOUT
 * *****************************************************************/

/*
 * Pre:
 * Post:
 */
bool verificaciones_alguien_logeado(user_t* user_logeado);

/*
 * Pre:
 * Post:
 */
void user_logout(user_t* user_logeado);


/* *****************************************************************
 *                     PUBLICAR UN POST
 * *****************************************************************/

/*
 * Pre:
 * Post:
 */
void publicar_post(user_t* user_logeado, hash_t* users, hash_t* publicaciones, char* mensaje);

/*
 * Pre:
 * Post:
 */
void publicacion_a_users(publicacion_t* publicacion, hash_t* users);


/* *****************************************************************
 *                     VER PROXIMO POST
 * *****************************************************************/

/*
 * Pre:
 * Post:
 */
bool verificaciones_ver_proximo(user_t* user_logeado);

/*
 * Pre:
 * Post:
 */
void ver_proximo_post(user_t* user_logeado);


/* *****************************************************************
 *                     LIKEAR UN POST
 * *****************************************************************/

/*
 * Pre:
 * Post:
 */
bool verificaciones_likear_post(user_t* user_logeado, size_t id, hash_t* publicaciones);

/*
 * Pre:
 * Post:
 */
void likear_post(user_t* user_logeado, size_t id, hash_t* publicaciones);

/* *****************************************************************
 *                     MOSTRAR LIKES
 * *****************************************************************/

/*
 * Pre:
 * Post:
 */
bool verificaciones_ver_likes(user_t* user_logeado, size_t id, hash_t* publicaciones);

/*
 * Pre:
 * Post:
 */
void mostrar_likes(user_t* user_logeado, size_t id, hash_t* publicaciones);


#endif //_ALGO2__FUNCIONES_H
