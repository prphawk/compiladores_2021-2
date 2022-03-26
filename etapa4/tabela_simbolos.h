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

typedef struct entradaArgumento
{
    char *nome;
    int tamanho;
    TipoSimbolo tipo_simbolo;
    struct EntradaArgumento *proximo;
} EntradaArgumento;

typedef struct entradaHashSimbolo
{
    int linha;
    int tamanho;
    char *chave;
    TipoSimbolo tipo_simbolo;
    NaturezaSimbolo natureza_simbolo;
    EntradaArgumento argumentos;
    valorLexico valor_lexico;
    struct EntradaHashSimbolo *proximo;
} EntradaHashSimbolo;

typedef struct pilhaHash {
    EntradaHashSimbolo *topo[TAMANHO_HASH];
    struct PilhaHash *resto;
} PilhaHash;

PilhaHash pilha_hash;