#include "AlgoGram.h"
#define TAM_MAXIMO 160
#define ERROR_ARCHIVO 1


void ejecutar_algo_gram(algo_gram_t* algo_gram){
    char comando[TAM_MAXIMO];
    while (scanf("%s", comando) != EOF){
        ejecutar_comando(algo_gram, comando);
    }
}

#ifndef _CORRECTOR_

int main(int argc, char* argv[]) {
    FILE* archivo = resultado_archivo(argc, argv);
    if(!archivo){
        return ERROR_ARCHIVO;
    }
    hash_t* users = user_a_hash(archivo);
    hash_t* publicaciones_totales = hash_crear(f_destruir_publicacion);
    algo_gram_t* algo_gram = algo_gram_crear(users, publicaciones_totales);

    ejecutar_algo_gram(algo_gram);

    hash_destruir(users);
    hash_destruir(publicaciones_totales);
    algo_gram_destruir(algo_gram);
    return 0;
}

#endif // _CORRECTOR_