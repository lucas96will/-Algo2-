#ifndef _ALGO2__FUNCIONES_H
#define _ALGO2__FUNCIONES_H
#include "user.h"
#include "publicacion.h"
#include "publicacion_user.h"
#include "hash.h"
#include <stdio.h>

/* *****************************************************************
 *                          FUNCIONES
 * *****************************************************************/

/* WRAPPER DE DESTRUCCION DE USER_T
 * Pre: User creado
 * Post: Llama a la funcion de destruccion de user
 * libera la memoria asociada a user
 */
void f_destruir_user(void* user);

/* WRAPPER DE DESTRUCCION DE PUBLICACION_T
 * Pre: Publicacion creada
 * Post: Llama a la funcion de destruccion de publicacion
 * libera la memoria asociada a publicacion
 */
void f_destruir_publicacion(void* publicacion);

/*
 * Pre: publicacion_user_1 y 2 deben estar creadas
 * Post: Devuelve la diferencia entre las afinidades de publicacion_user_1 y 2
 */
int comparar_afinidad(const void* publicacion_user_1, const void* publicacion_user_2);

/*
 * Pre:
 * Post: Devuelve el archivo, si no se pudo abrir el archivo
 * por no encontrarse la direccion o por cantidad erronea de
 * parametros imprime por stdout:
 * "Error: Cantidad erronea de parametros"
 * "Error: archivo fuente inaccesible"
 * devuelve NULL.
 */
FILE* resultado_archivo(int cant_argumentos, char** argumentos);

/* Funcion para obtener el hashmap de los usuarios basado en el archivo
 * pasado por parametro
 * Pre: el archivo es valido (!= NULL)
 * Post: Devuelve un hashmap(clave = nombre, valor = usuario_t)
 * y cierra el archivo
 */
hash_t* user_a_hash(FILE* archivo);

/* *****************************************************************
 *                          USER LOGIN
 * *****************************************************************/


/* user = cadena de caracteres pedida por stdin
 * Pre: users creado, user_logeado creado
 * Post: Devuelve el usuario logeado
 * Casos: 1. Hay un usuario logeado -> devuelve el usuario_logeado
 *        2. No hay un usuario logeado
 *          2.1 User es encontrado en el hashmap -> devuelve el user encontrado
 *          2.2 User no es encontrado en el hashmap -> Devuelve NULL
 *        3. No se pudo logear por otra razon -> Devuelve NULL
 */
user_t* user_login(hash_t* users, char* user, user_t* user_logeado);

/* *****************************************************************
 *                          USER LOGOUT
 * *****************************************************************/


/*
 * Pre:
 * Post: Imprime por stdout acorde si hay un user_logeado previamente
 * o en el otro caso. Devuelve NULL
 */
void* user_logout(user_t* user_logeado);


/* *****************************************************************
 *                     PUBLICAR UN POST
 * *****************************************************************/

/* mensaje: cadena de caracteres ingresadas por stdin
 * Pre: user_logeado, users, publicaciones: estructuras creadas previamente
 * Post: Guarda en publicaciones la publicacion con los datos
 * del user logeado.
 * Si se pudo publicar post imprime "Post publicado"
 * en caso de que no se pudo crear imprime "Error: no habia usuario loggeado"
 *
 */
void publicar_post(user_t* user_logeado, hash_t* users, hash_t* publicaciones, char* mensaje);


/* *****************************************************************
 *                     VER PROXIMO POST
 * *****************************************************************/


/*
 * Pre: -
 * Post: Imprime el siguiente post (segun la afinidad del usuario)
 * Si no hay user logeado o no hay mas posts para ver
 * entonces imprime  "Usuario no loggeado o no hay mas posts para ver"
 */
void ver_proximo_post(user_t* user_logeado);


/* *****************************************************************
 *                     LIKEAR UN POST
 * *****************************************************************/

/* A traves del user logeado se likea el post con el id pasado por parametro
 * Pre: publicaciones: estructura creada previamente
 * Post: Se likea el post con el numero id dentro del hash publicaciones.
 * Si se pudo likear imprime -> "Post likeado"
 * Si no hay user logeado o id incorrecto imprime -> "Error: Usuario no loggeado o Post inexistente."
 */
void likear_post(user_t* user_logeado, size_t id, hash_t* publicaciones);

/* *****************************************************************
 *                     MOSTRAR LIKES
 * *****************************************************************/


/* Se muestra por pantalla los usuarios que le
 * dieron like a una publicacion con id pasado por parametro
 * Pre: Publicaciones: Estructura creada previamente
 * Post: Se imprimen por pantalla las personas que le dieron like
 * a la publicacion con id pasado por parametro.
 * Si no existe dicha publicacion o si el post no tiene likes
 * se imprime -> Error: Post inexistente o sin likes.
 */
void mostrar_likes(size_t id, hash_t* publicaciones);



#endif //_ALGO2__FUNCIONES_H