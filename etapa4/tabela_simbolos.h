#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "valor_lexico.h"

#define TAMANHO_INICIAL_HASH 500
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

typedef struct entradaHash
{
    char *chave;
    union conteudo {
        int linha;
        int coluna; //opcional, -1 quando n existe TODO o que ele quer dizer com coluna??
        int tamanho;
        TipoSimbolo tipo_simbolo;
        NaturezaSimbolo natureza_simbolo;
        ArgumentoFuncao argumentos;
        ValorLexico valor_lexico;
    } Conteudo;
} EntradaHash;

typedef struct pilhaHash {
    int capacidade;
    int quantidade_atual;
    EntradaHash **topo;
    struct PilhaHash *resto;
} PilhaHash;

// PilhaHash *pilha_hash = NULL;

char *chave(char *nome, NaturezaSimbolo natureza);
unsigned long indiceHash(char *chave);
EntradaHash *adicionaHash(NaturezaSimbolo natureza, TipoSimbolo tipo, ValorLexico valor_lexico);
EntradaHash *encontraNaPilha(char *chave, PilhaHash *pilha);
EntradaHash *encontraNaTabela(char *chave, EntradaHash **arr, int capacidade_hash);
void adicionaArgumento(EntradaHash entrada, TipoSimbolo tipo, int tamanho, ValorLexico valor_lexico);
void empilhaHash();
void desempilhaHash();
void liberaPilhaHash();

int probing(int indice, int capacidade_hash);
PilhaHash *expandeHash(PilhaHash *pilha);
