#include "AlgoGram.h"
#define TAM_MAXIMO 160



void ejecutar_algo_gram(algo_gram_t* algo_gram){
    char comando[TAM_MAXIMO];
    while (scanf("%s", comando) != EOF){
        ejecutar_comando(algo_gram, comando);
    }
}

#ifndef CORRECTOR

int main(int argc, char* argv[]) {

    algo_gram_t* algo_gram = algo_gram_crear(argc, argv);
    ejecutar_algo_gram(algo_gram);
    algo_gram_destruir(algo_gram);

    return 0;
}

#endif // CORRECTOR