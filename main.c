#include "AlgoGram.h"
//#include "funciones.h"
#include<signal.h>
#define TAM_MAXIMO 160
#define ERROR_ARCHIVO 1


int main(int argc, char* argv[]) {
    FILE* archivo = resultado_archivo(argc, argv);
    if(!archivo){
        return ERROR_ARCHIVO;
    }
    hash_t* users = user_a_hash(archivo);
    hash_t* publicaciones_totales = hash_crear(f_destruir_publicacion);
    algo_gram_t* algo_gram = crear_algo_gram(users, publicaciones_totales);

    char comando[TAM_MAXIMO];
    while (scanf("%s", comando) != EOF){
        if(comando_existe(algo_gram, comando)) {
            ejecutar_comando(algo_gram, comando);
        }
    }

    hash_destruir(users);
    hash_destruir(publicaciones_totales);
    destruir_algo_gram(algo_gram);
    return 0;
}