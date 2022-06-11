#include "funciones.h"
#include <string.h>
#define TAM_MAXIMO 160
#define ERROR_ARCHIVO 1

void _flush(void){
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int main(int argc, char* argv[]) {

    FILE* archivo = resultado_archivo(argc, argv);
    if(!archivo){
        return ERROR_ARCHIVO;
    }
    hash_t* users = user_a_hash(archivo);
    user_t* user_logeado = NULL;
    hash_t* publicaciones_totales = hash_crear(f_destruir_publicacion);

    char comando[TAM_MAXIMO];
    while (fscanf(stdin, "%s", comando) > 0){
        //Si se escribe por consola login -> esperar que se escriba un nombre y despues, (siendo user el nombre que se escribio por consola)
        if (strcmp(comando, "login") == 0 || strcmp(comando, "Login") == 0){
            char user[TAM_MAXIMO];
            scanf("%s", user);
            user_logeado = user_login(users, user, user_logeado);
        }
        
        //Si se escribe por consola logout
        if (strcmp(comando, "logout") == 0 || strcmp(comando, "Logout") == 0){
            user_logeado = user_logout(user_logeado);
        }

        //Si se escribe por consola publicar -> esperar que se escriba un mensaje y despues, (siendo mensaje el mensaje que se escribio por consola)
        if (strcmp(comando, "publicar") == 0 || strcmp(comando, "Publicar") == 0){
            char mensaje[TAM_MAXIMO];
            //scanf("%s", mensaje);
            getchar(); //Limpio buffer (quedaba un \n)
            //_flush();
            fscanf(stdin, "%[^\n]s", mensaje);
            publicar_post(user_logeado, users, publicaciones_totales, mensaje);
        }

        //Si se escribe por consola ver_siguiente_feed
        if (strcmp(comando, "ver_siguiente_feed") == 0 ){
            ver_proximo_post(user_logeado);
        }

        if (strcmp(comando, "likear_post") == 0){
            size_t id;
            scanf("%zu", &id);
            likear_post(user_logeado, id, publicaciones_totales);
        }


        //Si se escribe por consola likear_post -> esperar ID (siendo id lo que se escribio por conosla)
        if (strcmp(comando, "mostrar_likes") == 0){
            size_t id;
            scanf("%zu", &id);
            mostrar_likes(id, publicaciones_totales);
        }

    }

    hash_destruir(users);
    hash_destruir(publicaciones_totales);

    return 0;
}