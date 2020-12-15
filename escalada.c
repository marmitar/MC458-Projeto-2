/**
 * Tiago de Paula Alves - RA187679
 * Projeto de Algoritmo com Implementação 2
 * * * * * * * * * * * * * * * * * * * * * */

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

// Erro de Entrada Inválida.
#define ENTINV 0x1234


// Custo ao longo de um caminho.
typedef uint16_t custo_t;
// Maior custo possível.
#define CUSTO_MAX UINT16_MAX
// Impressão de custo.
#define PRIcusto PRIu16

// Custo (perigo) de uma posição,
typedef uint8_t perigo_t;


static attribute(pure, hot, nothrow)
custo_t min_custo(const perigo_t *parede, size_t n, size_t m);

static attribute(malloc, cold, nothrow)
perigo_t *leitura_parede(size_t *n, size_t *m);


/* * * * */
/* MAIN  */

static attribute(cold, nothrow)
/**
 * Apresenta o erro marcado em `errno` na saída de erro.
 */
void imprime_erro(const char *prog) {
    switch (errno) {
        case 0:
            fputs(prog, stderr);
            fputs(": erro desconhecido\n", stderr);
            break;
        case ENTINV:
            fputs(prog, stderr);
            fputs(": entrada inválida\n", stderr);
            break;
        default:
            perror(prog);
            break;
    }
}

/* Main */
int main(int argc, char const *argv[]) {
    size_t n, m;
    // leitura das dimensões e da matriz
    perigo_t *parede = leitura_parede(&n, &m);
    // problema de leitura
    if unlikely(parede == NULL) {
        imprime_erro(argv[0]);
        return EXIT_FAILURE;
    }

    // menor custo na parede
    custo_t custo = min_custo(parede, n, m);
    free(parede);
    // erro interno do algoritmo
    if unlikely(custo == CUSTO_MAX) {
        imprime_erro(argv[0]);
        return EXIT_FAILURE;
    }

    printf("%"PRIcusto"\n", custo);
    return EXIT_SUCCESS;
}


/* * * * * * */
/* ALGORITMO */

static inline attribute(const, hot, nothrow)
/**
 * Mínimo entre dois valores.
 */
custo_t min(custo_t a, custo_t b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

static attribute(pure, hot, nothrow)
/**
 * Encontra o custo do caminho de menro custo na parede.
 *
 * A matriz de custo `parede` deve ser representada
 * em row-major.
 *
 * Retorna `CUSTO_MAX` em caso de erro.
 */
custo_t min_custo(const perigo_t *parede, size_t n, size_t m) {
    // sem custo para matrizes vazias
    if unlikely(n == 0 || m == 0) {
        return 0;
    }

    // memorização do menor custo de i,j até o topo
    // representada em row-major
    custo_t *custo = malloc(n * m * sizeof(custo_t));
    if unlikely(custo == NULL) return CUSTO_MAX;

    // na última linha, o custo é apenas da própria célula
    for (size_t j = 0; j < m; j++) {
        const size_t i = n - 1;
        custo[m*i + j] = (custo_t) parede[m*i + j];
    }
    // as linhas seguintes (de cima para baixo) são calculadas
    // com base nos caminhos já calculados
    for (size_t i = n - 1; i > 0; i--) {
        // custo dos caminhos possíveis a partir da cél. atual
        // o caminho tem custo máximo quando é impossível
        custo_t ce = CUSTO_MAX;      // esquerda
        custo_t cs = CUSTO_MAX;      // superior
        custo_t cd = custo[m*i + 0]; // direita

        for (size_t j = 0; j < m - 1; j++) {
            size_t ij = m * (i - 1) + j;
            // custo da célula atual
            custo_t c0 = (custo_t) parede[ij];

            // custo dos caminhos possíveis a partir da cél. atual
            // o caminho tem custo máximo quando é impossível
            ce = cs; cs = cd;
            cd = custo[m*i + j+1];

            // o novo custo é o menor dos caminho e a célula atual
            custo[ij] = c0 + min(cs, min(ce, cd));
        }

        // última célula da linha
        ce = cs; cs = cd;
        // não tem caminho á direita
        cd = CUSTO_MAX;

        custo_t c0 = (custo_t) parede[m*(i-1) + m-1];
        custo[m*(i-1) + m-1] = c0 + min(cs, min(ce, cd));
    }

    // retorna o menor dos custos partindo da base da parede
    custo_t cmin = custo[m*0 + 0];
    for (size_t j = 1; j < m; j++) {
        cmin = min(cmin, custo[m*0 + j]);
    }

    free(custo);
    return cmin;
}


/* * * * * * * * * * */
/* LEITURA DOS DADOS */

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
        //`vscanf` para tratar argumentos variados
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

static attribute(malloc, cold, nothrow)
/**
 * Leitura das dimensões e dos custos da parede.
 *
 * A matriz é armazenada em row-major.
 *
 * Retorna NULL em caso de erro.
 */
perigo_t *leitura_parede(size_t *n, size_t *m) {
    if unlikely(!cscanf(2, "%zu %zu", n, m)) {
        // erro de leitura
        return NULL;
    }
    size_t N = *n, M = *m;
    if unlikely(N == 0 || M == 0) {
        // aloca um byte, para manter um ponteiro
        // válido para `free`
        return malloc(1);
    };

    perigo_t *custos = malloc(N * M * sizeof(perigo_t));
    // erro de alocação
    if unlikely(custos == NULL) return NULL;

    // leitura da matriz
    for (size_t i = 0; i < N * M; i++) {
        perigo_t custo;
        if unlikely(!cscanf(1, "%"SCNu8, &custo)) {
            // erro de leitura
            free(custos);
            return NULL;
        };

        custos[i] = custo;
    }
    return custos;
}
