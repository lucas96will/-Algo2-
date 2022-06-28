#include "AlgoGram.h"
#define CANTIDAD_COMANDOS 6
#define CANT_MAX_POST 10
#define ARCHIVO_MIN_ARGUMENTOS 2
#define ARGUMENTO_NOMBRE_ARCHIVO 1
#define TAM_MAXIMO 160

typedef enum{
    F_LOGIN = 0,
    F_LOGOUT,
    F_PUBLICAR,
    F_VER_SIGUIENTE_FEED,
    F_LIKEAR_POST,
    F_MOSTRAR_LIKES,
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


/* *****************************************************************
 *                       FUNCIONES AUXILIARES
 * *****************************************************************/

/*
 * Pre: La cadena de caracteres termina con un \n
 * Post: Borra el \n de la cadena de caracteres
 */
void borrar_salto_linea(char* str){
    if ((strlen(str) > 0) && (str[strlen(str) - 1] == '\n'))
    {
        str[strlen(str) - 1] = '\0';
    }
}

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
 * Pre:
 * Post: Devuelve el archivo, si no se pudo abrir el archivo
 * por no encontrarse la direccion o por cantidad erronea de
 * parametros imprime por stdout:
 * "Error: Cantidad erronea de parametros"
 * "Error: archivo fuente inaccesible"
 * devuelve NULL.
 */
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


/* Funcion para obtener el hashmap de los usuarios basado en el archivo
 * pasado por parametro
 * Pre: el archivo es valido (!= NULL)
 * Post: Devuelve un hashmap(clave = nombre, valor = usuario_t)
 */
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
        borrar_salto_linea(cadena);
        user_t* user = user_crear(cadena, contador);
        hash_guardar(usuarios, cadena, user);
        contador++;
        linea = getline(&cadena, &capacidad, archivo);
    }

    free(cadena);
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
bool se_puede_loggear(hash_t* users, char* user, user_t* user_logeado) {
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

    if (se_puede_loggear(users, user, user_logeado) == false) {
        return user_logeado;
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
bool hay_user_logeado(user_t* user_logeado) {
    return user_logeado != NULL;
}

void* user_logout(user_t* user_logeado) {
    if (hay_user_logeado(user_logeado) == false) {
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

        user_actualizar_feed(user_actual, publicacion, cant_users);
        hash_iter_avanzar(iter);
    }

    hash_iter_destruir(iter);
}

void publicar_post(user_t* user_logeado, hash_t* users, hash_t* publicaciones, char* mensaje) {
    if (hay_user_logeado(user_logeado) == false) {
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
    return (hay_user_logeado(user_logeado) && !user_feed_vacio(user_logeado));
}

void ver_proximo_post(user_t* user_logeado) {

    if (verificaciones_ver_proximo(user_logeado) == false) {
        fprintf(stdout, "%s", "Usuario no loggeado o no hay mas posts para ver\n");
        return;
    }
    publicacion_t* publicacion = (publicacion_t*)user_obtener_siguiente_feed(user_logeado);
    publicacion_imprimir(publicacion);
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
    if (hay_user_logeado(user_logeado) == false) {
        return false;
    }
    char* str_id = uinttostr(id);
    bool existe_post = hash_pertenece(publicaciones, str_id);
    free(str_id);
    return existe_post;
}

void likear_post(user_t* user_logeado, size_t id, hash_t* publicaciones) {

    if (verificaciones_likear_post(user_logeado, id, publicaciones) == false){
        fprintf(stdout, "%s", "Error: Usuario no loggeado o Post inexistente\n");
        return;
    }
    char* str_id = uinttostr(id);
    publicacion_t* publicacion = hash_obtener(publicaciones, str_id);
    free(str_id);
    publicacion_agregar_like(publicacion, user_logeado);
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

    return (publicacion != NULL && publicacion_cantidad_likes(publicacion) > 0);
}

void mostrar_likes(size_t id, hash_t* publicaciones) {
    if (verificaciones_ver_likes(id, publicaciones) == false){
        fprintf(stdout, "%s", "Error: Post inexistente o sin likes\n");
        return;
    }

    char* str_id = uinttostr(id);
    publicacion_t* publicacion = hash_obtener(publicaciones, str_id);
    free(str_id);
    publicacion_imprimir_likes(publicacion);
}

/* *****************************************************************
 *        FUNCIONES PRIMARIAS (wrappers para guardar en el hash)
 * *****************************************************************/

//Si se escribe por consola login
void _user_login(algo_gram_t* algo_gram){
    getchar();
    char* user = NULL;
    size_t capacidad = 0;
    ssize_t linea = getline(&user, &capacidad, stdin);
    if(linea != EOF){
        borrar_salto_linea(user);
    }
    algo_gram->user_logeado = user_login(algo_gram->users, user, algo_gram->user_logeado);
    free(user);
}

//Si se escribe por consola logout
void _user_logout(algo_gram_t* algo_gram){
    algo_gram->user_logeado = user_logout(algo_gram->user_logeado);
}

//Si se escribe por consola publicar
void _publicar_post(algo_gram_t* algo_gram){
    getchar(); //Limpio buffer (quedaba un \n)
    char* mensaje = NULL;
    size_t capacidad = 0;
    ssize_t linea = getline(&mensaje, &capacidad, stdin);
    if(linea != EOF){
        borrar_salto_linea(mensaje);
    }
    publicar_post(algo_gram->user_logeado, algo_gram->users, algo_gram->publicaciones, mensaje);
    free(mensaje);
}

//Si se escribe por consola ver_siguiente_feed
void _ver_proximo_post(algo_gram_t* algo_gram){
    ver_proximo_post(algo_gram->user_logeado);
}

//Si se escribe por consola likear_post
void _mostrar_likes(algo_gram_t* algo_gram){
    size_t id;
    if(scanf("%zu", &id) != EOF){
        mostrar_likes(id, algo_gram->publicaciones);
    }
}


//Si se escribe por consola mostrar_likes
void _likear_post(algo_gram_t* algo_gram){
    size_t id;
    if(scanf("%zu", &id) != EOF){
        likear_post(algo_gram->user_logeado, id, algo_gram->publicaciones);
    }
}

/* *****************************************************************
 *                          FUNCIONES AUXILIARES
 * *****************************************************************/

bool comandos_crear(algo_gram_t* algo_gram){
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

/* *****************************************************************
 *                          PRIMITIVAS
 * *****************************************************************/

algo_gram_t* algo_gram_crear(int argc, char* argv[]){
    algo_gram_t* algo_gram = malloc(sizeof(algo_gram_t));
    if(algo_gram == NULL){
        fprintf(stderr, "No se pudo crear algo_gram correctamente");
        return NULL;
    }
    if(comandos_crear(algo_gram) == false){
        free(algo_gram);
        fprintf(stderr, "No se pudo crear algo_gram correctamente");
        return NULL;
    }

    FILE* archivo = resultado_archivo(argc, argv);
    if(!archivo){
        fprintf(stderr, "No se pudo abrir el archivo");
        hash_destruir(algo_gram->comandos);
        free(algo_gram->funciones);
        return NULL;
    }

    algo_gram->users = user_a_hash(archivo);
    algo_gram->publicaciones = hash_crear(f_destruir_publicacion);
    algo_gram->user_logeado = NULL;

    fclose(archivo);

    return algo_gram;
}

/*
 * Pre: algo_gram creado previamente
 * Post: ejecuta el comando pasado por parametro.
 * Si el comando se ejecuto devuelve true, false en otro caso
 * (si no se encuentra en los comandos disponibles)
 */
bool ejecutar_comando(algo_gram_t* algo_gram, char* comando){
    if(hash_pertenece(algo_gram->comandos, comando) == false){
        return false;
    }
    funciones_t* func = hash_obtener(algo_gram->comandos, comando);
    func->f_comando(algo_gram);
    return true;
}

void algo_gram_destruir(algo_gram_t* algo_gram){
    hash_destruir(algo_gram->users);
    hash_destruir(algo_gram->publicaciones);
    free(algo_gram->funciones);
    hash_destruir(algo_gram->comandos);
    free(algo_gram);
}

void algo_gram_iniciar(algo_gram_t* algo_gram){
    char comando[TAM_MAXIMO];
    while (scanf("%s", comando) != EOF){
        ejecutar_comando(algo_gram, comando);
    }
}