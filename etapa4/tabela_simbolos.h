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

typedef struct conteudo {
    int linha;
    int coluna; //opcional, -1 quando n existe TODO o que ele quer dizer com coluna??
    int tamanho;
    TipoSimbolo tipo_simbolo;
    NaturezaSimbolo natureza_simbolo;
    ArgumentoFuncao* argumentos;
    ValorLexico valor_lexico;
} Conteudo;

typedef struct entradaHash
{
    char *chave;
    Conteudo conteudo;
    
} EntradaHash;

typedef struct pilhaHash {
    int quantidade_atual;
    int capacidade_tabela;
    EntradaHash *topo;
    struct PilhaHash *resto;
} PilhaHash;

char *chave(char *nome, NaturezaSimbolo natureza);
unsigned long indiceHash(char *chave);
EntradaHash *insereNoEscopo(NaturezaSimbolo natureza, TipoSimbolo tipo, ValorLexico valor_lexico);
EntradaHash *encontraNoEscopo(char *chave, PilhaHash *pilha);
EntradaHash *encontraNaTabela(char *chave, EntradaHash *tabela, int tamanho_tabela);
EntradaHash *insereNaTabela(char *chave, PilhaHash *pilha, Conteudo conteudo);
void adicionaArgumento(EntradaHash entrada, TipoSimbolo tipo, int tamanho, ValorLexico valor_lexico);
void empilhaHash();
void desempilhaHash();
void liberaTabela();

int probing(int indice, int capacidade_hash);
PilhaHash *expandeTabela(PilhaHash *pilha);
int tamanho(TipoSimbolo tipo);
void printTabela(int profundidade, EntradaHash *tabela, int tamanho_tabela);
void printEscopos();
EntradaHash *novaTabela();
void insere_literal_hash(TipoSimbolo tipo, ValorLexico valor_lexico);
void insere_funcao_hash(ValorLexico valor_lexico);
void insere_identificador_hash(TipoSimbolo tipo, ValorLexico valor_lexico);
