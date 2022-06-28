#include "AlgoGram.h"

#ifndef CORRECTOR

int main(int argc, char* argv[]) {

    algo_gram_t* algo_gram = algo_gram_crear(argc, argv);
    algo_gram_iniciar(algo_gram);
    algo_gram_destruir(algo_gram);

    return 0;
}

#endif // CORRECTOR