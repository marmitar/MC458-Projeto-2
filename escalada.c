/**
 * Este arquivo pode ser usado como base para o seu codigo.
 * Preste especial atencao a funcao ja implementada "imprimeSaida". Esta função
 * deve ser usada para imprimir o resultado da execucao de cada algoritmo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>


// Lista de atributos da função.
#define attribute(...) \
    __attribute__((__VA_ARGS__))

// Marcador de branch provável.
#define likely(x) \
    (__builtin_expect((x), 1))
// Marcador de branch improvável.
#define unlikely(x) \
    (__builtin_expect((x), 0))


typedef uint64_t custo_t;

typedef struct parede {
    size_t n, m;
    uint8_t *dados;
} parede_t;

static inline attribute(pure)
custo_t custo_em(const parede_t parede, size_t i, size_t j) {
    size_t pos = i * parede.n + j;
    return (custo_t) parede.dados[pos];
}

#define ENTINV 0x1234

static
int ler_parede(parede_t *parede) {
    size_t n, m;
    int rv = scanf("%zu %zu", &n, &m);
    if (rv < 0) return errno;
    if (rv < 2) return ENTINV;

    uint8_t *dados = malloc(n * m * sizeof(uint8_t));
    if unlikely(dados == NULL) return errno;

    for (size_t i = 0; i < n * m; i++) {
        uint8_t custo;
        rv = scanf("%"SCNu8, &custo);
        if unlikely(rv < 0) {
            free(dados);
            return errno;
        } else if unlikely(rv < 1) {
            free(dados);
            return ENTINV;
        }

        dados[i] = custo;
    }

    parede->n = n;
    parede->m = m;
    parede->dados = dados;
    return 0;
}

/**
 * Imprime o custo do caminho.
 */
void imprime_saida(custo_t custo) {
    printf("%"PRIu64"\n", custo);
}

void imprime_erro(const char *prog, int err) {
    switch (err) {
        case 0:
            fprintf(stderr, "%s: erro desconhecido\n", prog);
            break;
        case ENTINV:
            fprintf(stderr, "%s: entrada inválida\n", prog);
            break;
        default:
            perror(prog);
            break;
    }
}

int main(int argc, char const *argv[]) {
    parede_t parede;
    int rv = ler_parede(&parede);
    if (rv != 0) {
        imprime_erro(argv[0], rv);
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}
