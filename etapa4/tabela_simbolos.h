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
    TIPO_INTEIRO,
    TIPO_FLOAT,
    TIPO_BOOL,
    TIPO_CHAR,
    TIPO_STRING,
    TIPO_OUTRO
} TipoSimbolo;

typedef enum natureza_simbolo
{
    NATUREZA_LITERAL,
    NATUREZA_VARIAVEL,
    NATUREZA_FUNCAO
} NaturezaSimbolo;

typedef struct argumentoFuncao
{
    // char *nome;
    // int tamanho;
    TipoSimbolo tipo;
    struct ArgumentoFuncao *proximo;
} ArgumentoFuncao;

typedef struct conteudo {
    int linha;
    int coluna; //opcional, -1 quando n existe TODO o que ele quer dizer com coluna??
    int tamanho;
    TipoSimbolo tipo;
    NaturezaSimbolo natureza;
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

char *chave(char *nome, NaturezaSimbolo natureza, TipoSimbolo tipo);
unsigned long indice_hash(char *chave);
EntradaHash *insere_no_escopo(NaturezaSimbolo natureza, TipoSimbolo tipo, ValorLexico valor_lexico);
EntradaHash *encontra_no_escopo(char *chave, PilhaHash *pilha);
EntradaHash *encontra_na_tabela(char *chave, EntradaHash *tabela, int tamanho_tabela);
EntradaHash *insere_na_tabela(char *chave, PilhaHash *pilha, Conteudo conteudo);
void adiciona_argumento(EntradaHash entrada, TipoSimbolo tipo, int tamanho, ValorLexico valor_lexico);
void empilha();
void desempilha();
void libera_tabela();

int probing(int indice, int capacidade_hash);
PilhaHash *expande_tabela(PilhaHash *pilha);
int tamanho(TipoSimbolo tipo);
void print_tabela(EntradaHash *tabela, int tamanho_tabela);
void print_escopos();
EntradaHash *adiciona_tabela();
void insere_literal_tabela(TipoSimbolo tipo, ValorLexico valor_lexico);
void insere_funcao_tabela(ValorLexico valor_lexico);
void insere_identificador_tabela(TipoSimbolo tipo, ValorLexico valor_lexico);
void libera_argumentos(ArgumentoFuncao *argumento);
void libera_pilha();
char* print_tipo(TipoSimbolo tipo);
char* print_natureza(NaturezaSimbolo natureza);