#define _POSIX_C_SOURCE 200809L
#include "funciones.h"
#include <string.h>
#include <stdlib.h>

#define CANT_MAX_POST 10



#define ARCHIVO_MIN_ARGUMENTOS 2
#define ARGUMENTO_NOMBRE_ARCHIVO 1

/*
 * Pre: -
 * Post: devuelve una cadena de caracteres representando el numero
 *       en memoria dinamica
 */
char* uinttostr(size_t num){
    char* str = malloc(sizeof(char)*CANT_MAX_POST);
    if(str == NULL){
        return NULL;
    }
    sprintf(str, "%zu", num);

    return str;
}


/* WRAPPER DE DESTRUCCION DE USER_T
 * Pre: User creado
 * Post: Llama a la funcion de destruccion de user
 * libera la memoria asociada a user
 */
void f_destruir_user(void* user){
    user_destruir((user_t*)user);
}

/* WRAPPER DE DESTRUCCION DE PUBLICACION_T
 * Pre: Publicacion creada
 * Post: Llama a la funcion de destruccion de publicacion
 * libera la memoria asociada a publicacion
 */
void f_destruir_publicacion(void* publicacion){
    publicacion_destruir((publicacion_t*)publicacion);
}

/*
 * pre: user1 y user2 creados.
 * post: devuelve un numero que representa la afinidad entre los dos usuarios
 * */
size_t calculo_afinidad(user_t* user1, user_t* user2, size_t id_user) {
    int diferencia = abs(((int)user_obtener_id(user1) - (int)user_obtener_id(user2)));
    size_t afinidad = id_user - (size_t)diferencia;
    return afinidad;
}


FILE* resultado_archivo(int cant_argumentos, char** argumentos) {

    if (cant_argumentos < ARCHIVO_MIN_ARGUMENTOS) {
        fprintf(stdout, "%s", "Error: Cantidad erronea de parametros\n");
        return NULL;
    }
    char* ruta = argumentos[ARGUMENTO_NOMBRE_ARCHIVO];

    FILE* archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        fprintf(stdout, "%s", "Error: archivo fuente inaccesible\n");
        return NULL;
    }

    return archivo;
}


// Podriamos incluirla en las funciones del struct user
hash_t* user_a_hash(FILE* archivo) {
    
    hash_t* usuarios = hash_crear(f_destruir_user);
    if (!usuarios) {
        return NULL;
    }

    char* cadena = NULL;
    size_t capacidad = 0;
    ssize_t linea = getline(&cadena, &capacidad, archivo);

    size_t contador = 0;

    while(linea != EOF) {
        // Remover salto de línea
        if ((strlen(cadena) > 0) && (cadena[strlen(cadena) - 1] == '\n'))
        {
            cadena[strlen(cadena) - 1] = '\0';
        }
        user_t* user = user_crear(cadena, contador);
        hash_guardar(usuarios, cadena, user);
        contador++;
        linea = getline(&cadena, &capacidad, archivo);
    }

    free(cadena);
    fclose(archivo);
    return usuarios;
}

/* *****************************************************************
 *                          USER LOGIN
 * *****************************************************************/

/*
 * Pre: users y user_logeado creado
 * Post: Devuelve true si se puede logear el user (se encuentra en el hash de users
 * y no hay user_logeado), false en otro caso
 */
bool verificaciones_login(hash_t* users, char* user, user_t* user_logeado) {
    if (user_logeado != NULL) {
        fprintf(stdout, "%s", "Error: Ya habia un usuario loggeado\n");
        return false;
    }
    if (!hash_pertenece(users, user)){
        fprintf(stdout, "%s", "Error: usuario no existente\n");
        return false;
    }

    return true;
}

user_t* user_login(hash_t* users, char* user, user_t* user_logeado) {

    if (verificaciones_login(users, user, user_logeado) == false) {
        return user_logeado == NULL ? NULL: user_logeado;
    }

    user_t* user_login = (user_t*)hash_obtener(users, user);

    fprintf(stdout, "Hola %s\n", user);
    return user_login;
}

/* *****************************************************************
 *                          USER LOGOUT
 * *****************************************************************/

/*
 * Pre:
 * Post: Devuelve true si hay un user_logeado, false en otro caso
 */
bool verificaciones_alguien_logeado(user_t* user_logeado) {
    return user_logeado != NULL;
}

void* user_logout(user_t* user_logeado) {
    if (verificaciones_alguien_logeado(user_logeado) == false) {
        fprintf(stdout, "%s", "Error: no habia usuario loggeado\n");
        return NULL;
    }

    fprintf(stdout, "%s", "Adios\n");
    return NULL;
}

/* *****************************************************************
 *                     PUBLICAR UN POST
 * *****************************************************************/

/*
 * Pre: publicacion, users: estructuras creadas previamente
 * Post: A todos los users se les guarda la publicacion
 * Complejidad: O(ulogp)  u: cantidad de usuarios, p: cantidad de publicaciones
 */
void publicacion_a_users(publicacion_t* publicacion, hash_t* users) {

    user_t* user_publico = publicacion_obtener_user(publicacion);
    hash_iter_t* iter = hash_iter_crear(users);
    size_t cant_users = hash_cantidad(users);

    while (!hash_iter_al_final(iter)) {
        const char* clave_actual = hash_iter_ver_actual(iter);
        user_t* user_actual = (user_t*)hash_obtener(users, clave_actual);

        if (user_actual == user_publico) {
            hash_iter_avanzar(iter);
            continue;
        }
        size_t afinidad = calculo_afinidad(user_publico, user_actual, cant_users);
        publicacion_user_t* publicacion_user = publicacion_user_crear(publicacion, afinidad);
        user_actualizar_feed(user_actual, publicacion_user);
        hash_iter_avanzar(iter);
    }

    hash_iter_destruir(iter);
}

void publicar_post(user_t* user_logeado, hash_t* users, hash_t* publicaciones, char* mensaje) {
    if (verificaciones_alguien_logeado(user_logeado) == false) {
        fprintf(stdout, "%s", "Error: no habia usuario loggeado\n");
        return;
    }
    size_t id = hash_cantidad(publicaciones);
    char* str_id = uinttostr(id);
    publicacion_t* publicacion = publicacion_crear(user_logeado, mensaje, id);
    hash_guardar(publicaciones, str_id, publicacion);
    publicacion_a_users(publicacion, users);
    fprintf(stdout, "Post publicado\n");
    free(str_id);
}



/* *****************************************************************
 *                     VER PROXIMO POST
 * *****************************************************************/

/*
 * Pre: -
 * Post: Devuelve true si hay mas publicaciones para ver del user logeado
 * devuelve false si no hay user logeado o si no hay mas publicaciones para ver
 */
bool verificaciones_ver_proximo(user_t* user_logeado) {
    if (verificaciones_alguien_logeado(user_logeado) == false) {
        return false;
    }

    if (user_feed_vacio(user_logeado)) {
        return false;
    }

    return true;
}

void ver_proximo_post(user_t* user_logeado) {

    if (verificaciones_ver_proximo(user_logeado) == false) {
        fprintf(stdout, "%s", "Usuario no loggeado o no hay mas posts para ver\n");
        return;
    }
    publicacion_user_t* publicacion_user = user_obtener_siguiente_feed(user_logeado);
    publicacion_t* publicacion = publicacion_user_obtener_publicacion(publicacion_user);

    publicacion_imprimir(publicacion);

    publicacion_user_destruir(publicacion_user);
}

/* *****************************************************************
 *                     LIKEAR UN POST
 * *****************************************************************/

/*
 * Pre: Publicaciones: estructura creada previamente
 * Post: Devuelve true si hay un user logeado y existe la publicacion
 * con el id dentro de todas las publicaciones.
 * Devuelve false si no hay user logeado o si no existe la publicacion
 */
bool verificaciones_likear_post(user_t* user_logeado, size_t id, hash_t* publicaciones) {
    if (verificaciones_alguien_logeado(user_logeado) == false) {
        return false;
    }
    char* str_id = uinttostr(id);
    if (!hash_pertenece(publicaciones, str_id)) {
        free(str_id);
        return false;
    }
    free(str_id);
    return true;
}

void likear_post(user_t* user_logeado, size_t id, hash_t* publicaciones) {

    if (verificaciones_likear_post(user_logeado, id, publicaciones) == false){
        fprintf(stdout, "%s", "Error: Usuario no loggeado o Post inexistente\n");
        return;
    }
    char* str_id = uinttostr(id);
    publicacion_t* publicacion = hash_obtener(publicaciones, str_id);
    publicacion_agregar_like(publicacion, user_logeado);
    free(str_id);
    fprintf(stdout, "%s", "Post likeado\n");
}


/* *****************************************************************
 *                     MOSTRAR LIKES
 * *****************************************************************/

/*
 * Pre: Publicaciones: estructura creada previamente
 * Post: Devuelve true si se hallo una publicacion con id dentro de publicaciones
 * y tiene algun like. False en otro caso
 */
bool verificaciones_ver_likes(size_t id, hash_t* publicaciones){
    char* str_id = uinttostr(id);
    publicacion_t* publicacion = hash_obtener(publicaciones, str_id);
    free(str_id);
    if(publicacion == NULL || publicacion_cantidad_likes(publicacion) == 0){
        return false;
    }

    return true;
}

void mostrar_likes(size_t id, hash_t* publicaciones) {
    if (verificaciones_ver_likes(id, publicaciones) == false){
        fprintf(stdout, "%s", "Error: Post inexistente o sin likes\n");
        return;
    }

    char* str_id = uinttostr(id);
    publicacion_t* publicacion = hash_obtener(publicaciones, str_id);
    free(str_id);
    abb_t* likes = publicacion_obtener_likes(publicacion);
    size_t cant_likes = publicacion_cantidad_likes(publicacion);
    fprintf(stdout, "El post tiene %ld likes:\n", cant_likes);

    abb_iter_t* iter = abb_iter_in_crear(likes);

    while (!abb_iter_in_al_final(iter)) {
        const char* nombre = abb_iter_in_ver_actual(iter);
        fprintf(stdout, "\t%s\n", nombre);
        abb_iter_in_avanzar(iter);
    }

    abb_iter_in_destruir(iter);
}