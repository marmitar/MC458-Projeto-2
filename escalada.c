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

static attribute(pure, hot, nothrow)
custo_t min_custo(const uint8_t *parede, size_t n, size_t m) {
    if unlikely(n == 0 || m == 0) {
        return 0;
    }

    custo_t *custo = malloc(n * m * sizeof(custo_t));
    if unlikely(custo == NULL) return UINT32_MAX;

    for (size_t j = 0; j < m; j++) {
        const size_t fim = n - 1;
        custo[pos(m, fim, j)] = (custo_t) parede[pos(m, fim, j)];
    }

    for (size_t i = n - 1; i > 0; i++) {
        for (size_t j = 0; j < m; j++) {

        }
    }

    free(custo);
    return UINT32_MAX;
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

int main(int argc, char const *argv[]) {
    size_t n, m;
    uint8_t *parede = ler_parede(&n, &m);
    if unlikely(parede == NULL) {
        imprime_erro(argv[0]);
        return EXIT_FAILURE;
    }

    custo_t custo = 0;
    free(parede);


    if unlikely(custo == UINT32_MAX) {
        imprime_erro(argv[0]);
        return EXIT_FAILURE;
    }

    printf("%"PRIu32"\n", custo);
    return EXIT_SUCCESS;
}
