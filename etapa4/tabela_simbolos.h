#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "valor_lexico.h"

#define TAMANHO_HASH 500
#define TAMANHO_CHAR 1 
#define TAMANHO_INT 4
#define TAMANHO_FLOAT 8 

typedef enum tipo_simbolo
{
    SIMBOLO_TIPO_INTEIRO,
    SIMBOLO_TIPO_FLOAT,
    SIMBOLO_TIPO_BOOL,
    SIMBOLO_TIPO_CHAR,
    SIMBOLO_TIPO_STRING,
    SIMBOLO_TIPO_OUTRO
} TipoSimbolo;

typedef enum natureza_simbolo
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
        ValorLexico valor_lexico;
    } Conteudo;
} EntradaHashSimbolo;

typedef struct pilhaHash {
    EntradaHashSimbolo *topo[TAMANHO_HASH];
    struct PilhaHash *resto;
} PilhaHash;

PilhaHash pilha_hash;

char *chave(char *nome, NaturezaSimbolo natureza);
int indiceHash(char *chave);
EntradaHashSimbolo *adicionaHash(NaturezaSimbolo natureza, TipoSimbolo tipo, int tamanho, ValorLexico valor_lexico);
EntradaHashSimbolo *entradaHash(char *chave);
void adicionaArgumento(EntradaHashSimbolo entrada, TipoSimbolo tipo, int tamanho, ValorLexico valor_lexico);
void empilhaHash();
void desempilhaHash();
void liberaPilhaHash();
