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


/* * * * * */
/* MACROS  */

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


/* * * * * * * * * */
/* TIPOS ESPECIAIS */

// Largura dos inteiros de custo e risco.
#define CUSTO_WIDTH  8
#define RISCO_WIDTH 16

// Algutinação de tokens.
#define JOIN_(P, X, S) P ## X ## S
#define JOIN(P, X, S) JOIN_(P, X, S)

// Tipo do custo (perigo) de uma posição.
typedef JOIN(uint,CUSTO_WIDTH,_t) custo_t;
// Leitura de custo
#define SCNcusto JOIN(SCNu,CUSTO_WIDTH,)

// Tipo do risco (custo total) de um caminho.
typedef JOIN(uint,RISCO_WIDTH,_t) risco_t;
// Maior valor possível de risco.
#define RISCO_MAX JOIN(UINT,RISCO_WIDTH,_MAX)
// Impressão de risco.
#define PRIrisco JOIN(PRIu,CUSTO_WIDTH,)


/* * * * */
/* MAIN  */

static attribute(pure, hot, nothrow)
risco_t min_risco(const custo_t *parede, size_t n, size_t m);

static attribute(malloc, cold, nothrow)
custo_t *leitura_parede(size_t *n, size_t *m);

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
    // leitura das dimensões e da grade
    custo_t *parede = leitura_parede(&n, &m);
    // problema de leitura
    if unlikely(parede == NULL) {
        imprime_erro(argv[0]);
        return EXIT_FAILURE;
    }

    // menor risco na parede
    risco_t risco = min_risco(parede, n, m);
    free(parede);
    // erro interno do algoritmo
    if unlikely(risco == RISCO_MAX) {
        imprime_erro(argv[0]);
        return EXIT_FAILURE;
    }

    printf("%"PRIrisco"\n", risco);
    return EXIT_SUCCESS;
}


/* * * * * * */
/* ALGORITMO */

static inline attribute(const, hot, nothrow)
/**
 * Mínimo entre dois valores.
 */
risco_t min(risco_t a, risco_t b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

static attribute(pure, hot, nothrow)
/**
 * Encontra o risco do caminho ótimo na parede.
 *
 * A grade de custo deve ser representada em row-major.
 * Retorna `RISCO_MAX` em caso de erro.
 */
risco_t min_risco(const custo_t *custo, size_t n, size_t m) {
    // sem custo para matrizes vazias
    if unlikely(n == 0 || m == 0) {
        return 0;
    }

    // memorização do menor risco de i,j até o topo
    // representada em row-major
    risco_t *risco = malloc(n * m * sizeof(risco_t));
    if unlikely(risco == NULL) return RISCO_MAX;

    // na última linha, o risco é apenas da própria célula
    for (size_t j = 0; j < m; j++) {
        const size_t i = n - 1;
        risco[m*i + j] = (risco_t) custo[m*i + j];
    }
    // as linhas seguintes (de cima para baixo) são calculadas
    // com base nos caminhos já calculados
    for (size_t i = n - 1; i > 0; i--) {
        // risco dos caminhos possíveis a partir da cél. atual
        // o caminho tem risco máximo quando é impossível
        risco_t re = RISCO_MAX;      // esquerda
        risco_t rc = RISCO_MAX;      // central
        risco_t rd = risco[m*i + 0]; // direita

        for (size_t j = 0; j < m - 1; j++) {
            size_t ij = m * (i - 1) + j;
            // custo da célula atual
            risco_t cij = (risco_t) custo[ij];

            // caminhos possíveis baseados na célula anteriror
            re = rc; rc = rd;
            rd = risco[m*i + j+1];

            // o novo custo é o menor dos caminho e a célula atual
            risco[ij] = cij + min(rc, min(re, rd));
        }

        re = rc; rc = rd;
        // última célula da linha
        // não tem caminho á direita
        rd = RISCO_MAX;

        risco_t cij = (risco_t) custo[m*(i-1) + m-1];
        risco[m*(i-1) + m-1] = cij + min(rc, min(re, rd));
    }

    // retorna o menor dos risos partindo da base da parede
    risco_t rmin = risco[m*0 + 0];
    for (size_t j = 1; j < m; j++) {
        rmin = min(rmin, risco[m*0 + j]);
    }

    free(risco);
    return rmin;
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
 * Retorna NULL em caso de erro.
 */
custo_t *leitura_parede(size_t *N, size_t *M) {
    size_t n, m;
    if unlikely(!cscanf(2, "%zu %zu", &n, &m)) {
        // erro de leitura
        return NULL;
    }
    *N = n; *M = m;
    if unlikely(n == 0 || m == 0) {
        // aloca um byte, para manter um ponteiro
        // válido para `free`
        return malloc(1);
    };

    custo_t *custos = malloc(n * m * sizeof(custo_t));
    // erro de alocação
    if unlikely(custos == NULL) return NULL;

    // leitura da grade
    for (size_t i = 0; i < n * m; i++) {
        custo_t custo;
        if unlikely(!cscanf(1, "%"SCNcusto, &custo)) {
            // erro de leitura
            free(custos);
            return NULL;
        };

        custos[i] = custo;
    }
    return custos;
}
