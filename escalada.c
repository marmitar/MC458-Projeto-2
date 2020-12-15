/**
 * Este arquivo pode ser usado como base para o seu codigo.
 * Preste especial atencao a funcao ja implementada "imprimeSaida". Esta função
 * deve ser usada para imprimir o resultado da execucao de cada algoritmo.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
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


typedef uint32_t custo_t;

static inline attribute(const, hot, nothrow)
size_t pos(size_t m, size_t i, size_t j) {
    return i * m + j;
}

static inline attribute(const, hot, nothrow)
custo_t min(custo_t c1, custo_t c2, custo_t c3) {
    if (c1 < c2 && c1 < c3) {
        return c1;
    } else if (c2 < c3) {
        return c2;
    } else {
        return c3;
    }
}

static attribute(pure, hot, nothrow)
custo_t min_custo(const uint8_t *parede, size_t n, size_t m) {
    if unlikely(n == 0 || m == 0) {
        return 0;
    }

    custo_t *custo = malloc(n * m * sizeof(custo_t));
    if unlikely(custo == NULL) return UINT32_MAX;

    for (size_t j = 0; j < m; j++) {
        const size_t i = n - 1;
        custo[pos(m, i, j)] = (custo_t) parede[pos(m, i, j)];
    }

    for (size_t i = n - 1; i > 0; i--) {
        for (size_t j = 0; j < m; j++) {
            size_t ij = pos(m, i - 1, j);
            custo_t c0 = (custo_t) parede[ij];

            custo_t c1 = (j > 0)? custo[pos(m, i, j - 1)] : UINT32_MAX;
            custo_t c2 = custo[pos(m, i, j)];
            custo_t c3 = (j < m - 1)? custo[pos(m, i, j + 1)] : UINT32_MAX;

            custo[ij] = c0 + min(c1, c2, c3);
        }
    }

    custo_t cmin = custo[pos(m, 0, 0)];
    for (size_t j = 1; j < m; j++) {
        if (custo[pos(m, 0, j)] < cmin) {
            cmin = custo[pos(m, 0, j)];
        }
    }

    free(custo);
    return cmin;
}

#define ENTINV 0x1234

static attribute(format(scanf, 2, 3), nonnull, cold, nothrow)
/**
 * Checked `scanf`.
 *
 * Checa se o `scanf` fez todas as leituras esperadas,
 * como recebido pelo parâmetro `expect`.
 *
 * Retorna true em caso de sucesso. Para erros, o valor
 * do erro é marcado em `errno` e retorna false.
 */
bool cscanf(unsigned expect, const char *restrict fmt, ...) {
        va_list args;
        va_start(args, fmt);
        // usa `vfscanf` para tratar argumentos
        // variados com mais facilidade
        int rv = vscanf(fmt, args);
        va_end(args);

        // erro de leitura
        if unlikely(rv < 0) {
                return false;
        }
        // leitura incompleta
        else if unlikely(rv < expect) {
                errno = ENTINV;
                return false;
        }
        return true;
}


static inline attribute(malloc, cold, nothrow)
uint8_t *ler_dados(size_t n, size_t m) {
    if unlikely(n == 0 || m == 0) {
        return malloc(1);
    };

    uint8_t *dados = malloc(n * m * sizeof(uint8_t));
    if unlikely(dados == NULL) return NULL;

    for (size_t i = 0; i < n * m; i++) {
        uint8_t custo;
        if unlikely(!cscanf(1, "%"SCNu8, &custo)) {
            free(dados);
            return NULL;
        };

        dados[i] = custo;
    }
    return dados;
}

static attribute(cold, nothrow)
uint8_t *ler_parede(size_t *n, size_t *m) {
    if unlikely(!cscanf(2, "%zu %zu", n, m)) {
        return NULL;
    }

    return ler_dados(*n, *m);
}

static attribute(cold, nothrow)
void imprime_erro(const char *prog) {
    switch (errno) {
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

// void print_mat(const uint8_t *parede, size_t n, size_t m) {
//     printf("n=%zu,  m=%zu\n\n", n, m);

//     for (size_t i = 0; i < n; i++) {
//         printf("%zu:\t", i);
//         for (size_t j = 0; j < m; j++) {
//             printf(" %hhu", parede[pos(m, i, j)]);
//         }
//         printf("\n");
//     }
// }

int main(int argc, char const *argv[]) {
    size_t n, m;
    uint8_t *parede = ler_parede(&n, &m);
    if unlikely(parede == NULL) {
        imprime_erro(argv[0]);
        return EXIT_FAILURE;
    }
    // print_mat(parede, n, m);

    custo_t custo = min_custo(parede, n, m);
    free(parede);


    if unlikely(custo == UINT32_MAX) {
        imprime_erro(argv[0]);
        return EXIT_FAILURE;
    }

    printf("%"PRIu32"\n", custo);
    return EXIT_SUCCESS;
}
