#include "funciones.c"
#include <stdio.h>


int main(int argc, char* argv[]) {

    FILE* archivo = resultado_archivo(argc, argv);
    hash_t* users = user_a_hash(archivo);
    user_t* user_logeado = NULL;
    hash_t* publicaciones_totales = hash_crear(publicacion_destruir);

    //Si se escribe por consola login -> esperar que se escriba un nombre y despues, (siendo user el nombre que se escribio por consola)
    user_logeado = user_login(users, user, user_logeado);

    //Si se escribe por consola logout
    user_logout(user_logeado);

    //Si se escribe por consola publicar -> esperar que se escriba un mensaje y despues, (siendo mensaje el mensaje que se escribio por consola)
    publicar_post(user_logeado, users, publicaciones_totales, mensaje);

    //Si se escribe por consola ver_siguiente_feed
    ver_proximo_post(user_logeado);

    //Si se escribe por consola likear_post -> esperar ID (siendo id lo que se escribio por conosla)
    likear_post(user_logeado, id, publicaciones_totales);
    
    //Si se escribe por consola likear_post -> esperar ID (siendo id lo que se escribio por conosla)
    mostrar_likes(user_logeado, id, publicaciones_totales);
    
}