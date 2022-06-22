#ifndef _ALGO2__ALGOGRAM_H
#define _ALGO2__ALGOGRAM_H
#include "user.h"
#include "publicacion.h"
#include "hash.h"
#include <stdio.h>
#include <string.h>

// COMANDOS DISPONIBLES
#define LOGIN "login"
#define LOGOUT "logout"
#define PUBLICAR "publicar"
#define VER_SIGUIENTE_FEED "ver_siguiente_feed"
#define LIKEAR_POST "likear_post"
#define MOSTRAR_LIKES "mostrar_likes"

typedef struct algo_gram algo_gram_t;
typedef struct funciones funciones_t;

typedef void (*func_comando_t) (algo_gram_t* algo_gram);

/*
 * Pre: users, publicaciones: estructuras creadas previamente
 * Post: crea un algo_gram!
 */
algo_gram_t* algo_gram_crear(int argc, char* argv[]);

/*
 * Pre: algo_gram creado previamente
 * Post: ejecuta el comando pasado por parametro.
 * Si el comando se ejecuto devuelve true, false en otro caso
 * (si no se encuentra en los comandos disponibles)
 */
bool ejecutar_comando(algo_gram_t* algo_gram, char* comando);

/*
 * Pre: algo_gram creado previamente
 * Post: Libera la memoria reservada de algo_gram
 */
void algo_gram_destruir(algo_gram_t* algo_gram);


#endif //_ALGO2__ALGOGRAM_H
