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

EntradaHash *_insere_em_pilha(NaturezaSimbolo natureza, TipoSimbolo tipo, ValorLexico valor_lexico);
EntradaHash *_busca_pilha(char *chave, PilhaHash *pilha);
EntradaHash *_busca_topo_pilha(char *chave, PilhaHash *pilha);
EntradaHash *_insere_em_pilha_probing(char *chave, PilhaHash *pilha, Conteudo conteudo);
EntradaHash *_malloc_tabela();
PilhaHash *_malloc_expande_tabela(PilhaHash *pilha);
char *_chave(char *nome, NaturezaSimbolo natureza, TipoSimbolo tipo);
unsigned long _indice_hash(char *chave);
void _adiciona_argumento(EntradaHash entrada, TipoSimbolo tipo, int tamanho, ValorLexico valor_lexico);
int _probing(int indice, int capacidade_hash);
int _tamanho(TipoSimbolo tipo);
void _libera_tabela();
void _libera_argumentos(ArgumentoFuncao *argumento);
void libera_pilha();
char* _tipo_str(TipoSimbolo tipo);
char* _natureza_str(NaturezaSimbolo natureza);
void _print_tabela(EntradaHash *tabela, int tamanho_tabela);

void print_pilha();
void insere_literal_pilha(TipoSimbolo tipo, ValorLexico valor_lexico);
void insere_funcao_pilha(ValorLexico valor_lexico);
void insere_identificador_pilha(TipoSimbolo tipo, ValorLexico valor_lexico);

void empilha();
void desempilha();