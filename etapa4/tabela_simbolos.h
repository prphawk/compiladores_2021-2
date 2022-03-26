#include <stdio.h>
#include <string.h>
#include "valor_lexico.h"

#define TAMANHO_HASH 500

typedef enum tipoSimbolo
{
    SIMBOLO_TIPO_INTEIRO,
    SIMBOLO_TIPO_FLOAT,
    SIMBOLO_TIPO_BOOL,
    SIMBOLO_TIPO_CHAR,
    SIMBOLO_TIPO_STRING,
    SIMBOLO_TIPO_OUTRO
} TipoSimbolo;

typedef enum naturezaSimbolo
{
    SIMBOLO_LITERAL,
    SIMBOLO_VARIAVEL,
    SIMBOLO_FUNCAO
} NaturezaSimbolo;

typedef struct argumentoFuncao
{
    // char *nome;
    // int tamanho;
    TipoSimbolo tipo_simbolo;
    struct ArgumentoFuncao *proximo;
} ArgumentoFuncao;

typedef struct entradaHashSimbolo
{
    char *chave;
    struct EntradaHashSimbolo *proximo;
    union conteudo {
        int linha;
        int coluna; //opcional, -1 quando n existe
        int tamanho;
        TipoSimbolo tipo_simbolo;
        NaturezaSimbolo natureza_simbolo;
        ArgumentoFuncao argumentos;
        valorLexico valor_lexico;
    } Conteudo;
} EntradaHashSimbolo;

typedef struct pilhaHash {
    EntradaHashSimbolo *topo[TAMANHO_HASH];
    struct PilhaHash *resto;
} PilhaHash;

PilhaHash pilha_hash;