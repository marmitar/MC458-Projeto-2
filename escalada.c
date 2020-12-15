/**
 * Este arquivo pode ser usado como base para o seu codigo.
 * Preste especial atencao a funcao ja implementada "imprimeSaida". Esta função
 * deve ser usada para imprimir o resultado da execucao de cada algoritmo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>


#define attribute(...) \
    __attribute__((__VA_ARGS__))

typedef uint32_t custo_t;

typedef struct parede {
    size_t n, m;
    uint8_t *dados;
} parede_t;

static inline attribute(pure)
custo_t custo_em(const parede_t parede, size_t i, size_t j) {
    size_t pos = i * parede.n + j;
    return (custo_t) parede.dados[pos];
}

/**
 * Imprime o custo do caminho.
 */
void imprimeSaida(custo_t custo) {
    printf("%" PRIu32 "u\n", custo);
}

int main(int argc, char const *argv[]) {

    return EXIT_FAILURE;
}
