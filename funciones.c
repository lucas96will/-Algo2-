#include "struct.c"
#include <string.h>


size_t calculo_afinidad(user_t* user1, user_t* user2, size_t cant_user) { //Cant_user sera hash->cantidad del hash de usuarios
    size_t diferencia = abs((user1->id - user2->id));
    size_t afinidad = cant_user - diferencia;
    return afinidad;
}

//Para el heap del feed de cada usuario
int comparar_afinidad(const void* publicacion_user_1, const void* publicacion_user_2) {
    publicacion_user_t* p1 = (publicacion_user_t*) p1;
    publicacion_user_t* p2 = (publicacion_user_t*) p2;
    if(p1->afinidad < p2->afinidad) {
        return -1;
    } else if(p1->afinidad > p2->afinidad) {
        return 1;
    }
    return 0;
}

FILE* resultado_archivo(int cant_argumentos, char** argumentos) {

    if (cant_argumentos < 2) {
        fprintf(stdout, "%s", "Error: Cantidad erronea de parametros\n");
        return NULL;
    }
    char* ruta = argumentos[1];

    FILE* archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        fprintf(stdout, "%s", "Error: archivo fuente inaccesible\n");
        return NULL;
    }

    return archivo;
}


// Podriamos incluirla en las funciones del struct user
hash_t* user_a_hash(FILE* archivo) {
    
    hash_t* usuarios = hash_crear(user_destruir);
    if (!usuarios) {
        return NULL;
    }

    char* cadena = NULL;
    size_t capacidad = 0;
    ssize_t linea = getline(&cadena, &capacidad, archivo);

    size_t contador = 0;

    while(linea != EOF) {
        heap_t* heap_user = heap_crear(comparar_afinidad); 
        user_t* user = user_crear(cadena, contador, heap_user);
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

bool verificaciones_login(hash_t* users, char* user, user_t* user_logeado) {
    if (user_logeado != NULL) {
        fprintf(stdout, "%s", "Error: Ya habia un usuario loggeado\n");
        return false;
    }
    if (!hash_pertenece(users, user)){
        fprintf(stdout, "%s", "Error: Usuario no existente\n");
        return false;
    }

    return true;
}

user_t* user_login(hash_t* users, char* user, user_t* user_logeado) {

    if (verificaciones_login(users, user, user_logeado) == false) {
        return NULL;
    }

    user_t* user_login = (user_t*)hash_obtener(users, user);

    fprintf(stdout, "Hola %s", user);
    return user_login;
}

/* *****************************************************************
 *                          USER LOGOUT
 * *****************************************************************/

bool verificaciones_alguien_logeado(user_t* user_logeado) {
    if(!user_logeado) {
        fprintf(stdout, "%s", "Error: no habia usuario loggeado\n");
        return false;
    }
    return true;
}

void user_logout(user_t* user_logeado) {
    if (verificaciones_alguien_logeado(user_logeado) == false) {
        return;
    }

    user_logeado = NULL;

    fprintf(stdout, "%s", "Adios\n");
}

/* *****************************************************************
 *                     PUBLICAR UN POST
 * *****************************************************************/

void publicar_post(user_t* user_logeado, hash_t* users, hash_t* publicaciones, char* mensaje) {
    if (verificaciones_alguien_logeado(user_logeado) == false) {
        return;
    }

    size_t id = hash_cantidad(publicaciones);
    abb_t* likes = abb_crear(strcmp, NULL);
    publicacion_t* publicacion = publicacion_crear(user_logeado, mensaje, id, likes);
    hash_guardar(publicaciones, id, publicacion);
    publicacion_a_users(publicacion, users);
}

void publicacion_a_users(publicacion_t* publicacion, hash_t* users) {

    user_t* user_publico = publicacion->user;
    hash_iter_t* iter = hash_iter_crear(users);
    size_t cant_users = hash_cantidad(users);

    while (!hash_iter_al_final(iter)) { //O(u), siendo u la cantidad de usuarios
        char* clave_actual = hash_iter_ver_actual(iter);
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

/* *****************************************************************
 *                     VER PROXIMO POST
 * *****************************************************************/

bool verificaciones_ver_proximo(user_t* user_logeado) {
    if (verificaciones_alguien_logeado(user_logeado) == false) {
        return false;
    }

    if (heap_esta_vacio(user_logeado->feed)) {
        fprintf(stdout, "%s", "Error: no hay mas posts para ver\n");
        return false;
    }

    return true;
}

void ver_proximo_post(user_t* user_logeado) {

    if (verificaciones_ver_proximo(user_logeado) == false) {
        return;
    }
    publicacion_user_t* publicacion_user = heap_desencolar(user_logeado->feed);
    publicacion_t* publicacion = publicacion_user->publicacion;

    size_t id = publicacion->id;
    char* user_publico = publicacion->user->nombre;
    char* mensaje = publicacion->mensaje;
    size_t cant_likes = abb_cantidad(publicacion->likes);

    fprintf(stdout, "Post ID %d\n", id);
    fprintf(stdout, "%s dijo: %s\n", user_publico, mensaje);
    frpintf(stdout, "Likes: %d\n", cant_likes);
}

/* *****************************************************************
 *                     LIKEAR UN POST
 * *****************************************************************/

bool verificaciones_likear_post(user_t* user_logeado, size_t id, hash_t* publicaciones) {
    if (verificaciones_alguien_logeado(user_logeado) == false) {
        return false;
    }
    if (!hash_pertenece(publicaciones, id)) {
        fprintf(stdout, "%s", "Error: Post inexistente\n");
        return false;
    }
    return true;
}

void likear_post(user_t* user_logeado, size_t id, hash_t* publicaciones) {

    if (verificaciones_likear_post(user_logeado, id, publicaciones) == false){
        return;
    }

    publicacion_t* publicacion = hash_obtener(publicaciones, id);

    abb_guardar(publicacion->likes, user_logeado->nombre, user_logeado);
    fprintf(stdout, "%s", "Post likeado\n");
}


/* *****************************************************************
 *                     MOSTRAR LIKES
 * *****************************************************************/

bool verificaciones_ver_likes(user_t* user_logeado, size_t id, hash_t* publicaciones){
    if (verificaciones_likear_post(user_logeado, id, publicaciones) == false) {
        return false;
    }

    publicacion_t* publicacion = hash_obtener(publicaciones, id);
    abb_t* likes = publicacion->likes;
    if (abb_cantidad(likes) == 0) {
        fprintf(stdout, "%s", "Error: Sin likes\n");
        return false;
    }

    return true;
}

void mostrar_likes(user_t* user_logeado, size_t id, hash_t* publicaciones) {
    if (verificaciones_ver_likes(user_logeado, id, publicaciones) == false){
        return;
    }

    publicacion_t* publicacion = hash_obtener(publicaciones, id);
    abb_t* likes = publicacion->likes;
    size_t cant_likes = abb_cantidad(likes);
    fprintf(stdout, "El post tiene %d likes:\n", cant_likes);

    abb_iter_t* iter = abb_iter_in_crear(likes);

    while (!abb_iter_in_al_final(iter)) {
        const char* nombre = abb_iter_in_ver_actual(iter);
        frpintf(stdout, "   %s\n", nombre);
        abb_iter_in_avanzar(iter);
    }

    abb_iter_in_destruir(iter);
}
