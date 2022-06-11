#ifndef _ALGO2__ALGOGRAM_H
#define _ALGO2__ALGOGRAM_H

#include "funciones.h"
#include <string.h>

typedef struct algo_gram algo_gram_t;
typedef struct funciones funciones_t;

typedef void (*func_comando_t) (algo_gram_t* algo_gram);

/*
 * Pre: users, publicaciones: estructuras creadas previamente
 * Post: crea un algo_gram!
 */
algo_gram_t* crear_algo_gram(hash_t* users, hash_t* publicaciones);

/*
 * Devuelve un hash con los comandos disponibles
 * hash = (clave = comando, valor = funcion)
 */
hash_t* comandos_disponibles(algo_gram_t* algo_gram);

/*
 * Pre: algo_gram previamente creado
 * Post: devuelve si el comando existe en el algo_gram
 */
bool comando_existe(algo_gram_t* algo_gram, char* comando);

/*
 * Pre: algo_gram creado previamente
 * Post: ejecuta el comando pasado por parametro.
 * Si el comando se ejecuto devuelve true, false en otro caso
 * (si no se encuentra)
 */
bool ejecutar_comando(algo_gram_t* algo_gram, char* comando);

/*
 * Pre: algo_gram creado previamente
 * Post: Libera la memoria reservada de algo_gram
 */
void destruir_algo_gram(algo_gram_t* algo_gram);


#endif //_ALGO2__ALGOGRAM_H