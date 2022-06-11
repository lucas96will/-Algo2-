#include "AlgoGram.h"

#define TAM_MAXIMO 160


typedef enum{
    F_LOGIN = 0,
    F_LOGOUT,
    F_PUBLICAR,
    F_VER_SIGUIENTE_FEED,
    F_LIKEAR_POST,
    F_MOSTRAR_LIKES,
    MAX
} nombres_comandos_t;

struct funciones{
    func_comando_t f_comando;
};

struct algo_gram {
    hash_t* users;
    hash_t* publicaciones;
    hash_t* comandos;
    user_t* user_logeado;
    funciones_t* funciones;
};

#define CANTIDAD_COMANDOS 6


//Si se escribe por consola login -> esperar que se escriba un nombre y despues,
// (siendo user el nombre que se escribio por consola)
void _user_login(algo_gram_t* algo_gram){
    getchar();
    char* user = NULL;
    size_t capacidad = 0;
    getline(&user, &capacidad, stdin);
    if ((strlen(user) > 0) && (user[strlen(user) - 1] == '\n'))
    {
        user[strlen(user) - 1] = '\0';
    }
    algo_gram->user_logeado = user_login(algo_gram->users, user, algo_gram->user_logeado);
    free(user);
}

//Si se escribe por consola logout
void _user_logout(algo_gram_t* algo_gram){
    algo_gram->user_logeado = user_logout(algo_gram->user_logeado);
}

//Si se escribe por consola publicar -> esperar que se escriba un mensaje y despues,
// (siendo mensaje el mensaje que se escribio por consola)
void _publicar_post(algo_gram_t* algo_gram){
    getchar(); //Limpio buffer (quedaba un \n)
    char* mensaje = NULL;
    size_t capacidad = 0;
    getline(&mensaje, &capacidad, stdin);
    mensaje[strlen(mensaje)-1] = '\0';
    publicar_post(algo_gram->user_logeado, algo_gram->users, algo_gram->publicaciones, mensaje);
    free(mensaje);
}

//Si se escribe por consola ver_siguiente_feed
void _ver_proximo_post(algo_gram_t* algo_gram){
    ver_proximo_post(algo_gram->user_logeado);
}

//Si se escribe por consola likear_post -> esperar ID
// (siendo id lo que se escribio por conosla)
void _mostrar_likes(algo_gram_t* algo_gram){
    size_t id;
    scanf("%zu", &id);
    mostrar_likes(id, algo_gram->publicaciones);
}


//Si se escribe por consola mostrar_likes -> esperar ID
// (siendo id lo que se escribio por conosla)
void _likear_post(algo_gram_t* algo_gram){
    size_t id;
    scanf("%zu", &id);
    likear_post(algo_gram->user_logeado, id, algo_gram->publicaciones);
}

bool crear_comandos(algo_gram_t* algo_gram){
    hash_t* hash = hash_crear(NULL);
    funciones_t* funciones = malloc(sizeof(funciones_t) * CANTIDAD_COMANDOS);
    if(funciones == NULL) {
        hash_destruir(hash);
        return false;
    }
    // Asignacion de funciones
    funciones[F_LOGIN].f_comando = _user_login;
    funciones[F_LOGOUT].f_comando = _user_logout;
    funciones[F_PUBLICAR].f_comando = _publicar_post;
    funciones[F_VER_SIGUIENTE_FEED].f_comando = _ver_proximo_post;
    funciones[F_LIKEAR_POST].f_comando = _likear_post;
    funciones[F_MOSTRAR_LIKES].f_comando = _mostrar_likes;

    // Guardo en el hash las diferentes funciones
    hash_guardar(hash, LOGIN, &funciones[F_LOGIN]);
    hash_guardar(hash, LOGOUT, &funciones[F_LOGOUT]);
    hash_guardar(hash, PUBLICAR, &funciones[F_PUBLICAR]);
    hash_guardar(hash, VER_SIGUIENTE_FEED, &funciones[F_VER_SIGUIENTE_FEED]);
    hash_guardar(hash, LIKEAR_POST, &funciones[F_LIKEAR_POST]);
    hash_guardar(hash, MOSTRAR_LIKES, &funciones[F_MOSTRAR_LIKES]);

    algo_gram->comandos = hash;
    algo_gram->funciones = funciones;

    return true;
}


algo_gram_t* crear_algo_gram(hash_t* users, hash_t* publicaciones){
    algo_gram_t* algo_gram = malloc(sizeof(algo_gram_t));
    if(algo_gram == NULL){
        return NULL;
    }
    if(crear_comandos(algo_gram) == false){
        free(algo_gram);
        return NULL;
    }
    algo_gram->publicaciones = publicaciones;
    algo_gram->users = users;
    algo_gram->user_logeado = NULL;

    return algo_gram;
}

hash_t* comandos_disponibles(algo_gram_t* algo_gram){
    return algo_gram->comandos;
}

bool comando_existe(algo_gram_t* algo_gram, char* comando){
    return hash_pertenece(algo_gram->comandos, comando);
}

bool ejecutar_comando(algo_gram_t* algo_gram, char* comando){
    if(!hash_pertenece(algo_gram->comandos, comando)){
        return false;
    }
    funciones_t* func = hash_obtener(algo_gram->comandos, comando);
    func->f_comando(algo_gram);
    return true;
}

void destruir_algo_gram(algo_gram_t* algo_gram){
    free(algo_gram->funciones);
    hash_destruir(algo_gram->comandos);
    free(algo_gram);
}