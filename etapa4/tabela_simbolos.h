#include <stdio.h>
#include <string.h>
#include "valor_lexico.h"

#define TAMANHO_HASH 500

typedef enum tipoLiteral
{
    TIPO_INTEIRO,
    TIPO_FLOAT,
    TIPO_BOOL,
    TIPO_CHAR,
    TIPO_STRING,
    TIPO_OUTRO
} TipoSimbolo;

typedef enum tipoLiteral
{
    LITERAL,
    VARIAVEL,
    FUNCAO
} Natureza;

typedef struct entradaArgumento
{
    char *nome;
    TipoSimbolo tipo;
    int tamanho;
    entradaArgumento *proximo;
} entradaArgumento;

typedef struct entradaHashSimbolo
{
    char *chave;
    int linha;
    Natureza natureza;
    TipoSimbolo tipo;
    int tamanho;
    entradaArgumento argumentos;
    valorLexico valor_lexico;
    entradaHashSimbolo *proximo;
} entradaHashSimbolo;

typedef struct pilhaHash {
    entradaHashSimbolo *topo[TAMANHO_HASH];
    pilhaHash *resto;
} pilhaHash;

pilhaHash pilha_hash;