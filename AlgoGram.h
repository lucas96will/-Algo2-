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
 * Post: inicia algo_gram, tomando por consola los comandos disponibles
 */
void algo_gram_iniciar(algo_gram_t* algo_gram);

/*
 * Pre: algo_gram creado previamente
 * Post: Libera la memoria reservada de algo_gram
 */
void algo_gram_destruir(algo_gram_t* algo_gram);


#endif //_ALGO2__ALGOGRAM_H
