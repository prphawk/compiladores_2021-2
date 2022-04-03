#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "valor_lexico.h"
#include "errors.h"

#define TAMANHO_INICIAL_HASH 500
#define TAMANHO_CHAR 1 
#define TAMANHO_INT 4
#define TAMANHO_FLOAT 8

typedef enum tipo
{
    TIPO_INT,
    TIPO_FLOAT,
    TIPO_BOOL,
    TIPO_CHAR,
    TIPO_STRING,
    TIPO_OUTRO,
} Tipo;

typedef enum tipo_simbolo
{
    TIPO_PENDENTE = TIPO_OUTRO + 1
} TipoSimbolo;

typedef enum natureza_simbolo
{
    NATUREZA_LITERAL,
    NATUREZA_VARIAVEL,
    NATUREZA_VETOR,
    NATUREZA_FUNCAO
} NaturezaSimbolo;

typedef struct argumentoFuncaoLst
{
    // char *nome;
    // int tamanho;
    TipoSimbolo tipo;
    struct ArgumentoFuncaoLst *proximo;
} ArgumentoFuncaoLst;

typedef struct variavelSemTipoLst
{
    char *chave;
    int tamanho_vetor;
    struct VariavelSemTipoLst *proximo;
} VariavelSemTipoLst;

typedef struct conteudo {
    int linha;
    int coluna; //opcional, -1 quando n existe 
    //TODO o que ele quer dizer com coluna??
    int tamanho;
    TipoSimbolo tipo;
    NaturezaSimbolo natureza;
    ArgumentoFuncaoLst* argumentos;
    ValorLexico valor_lexico;
} Conteudo;

typedef struct entradaHash
{
    char *chave;
    Conteudo conteudo;
    
} EntradaHash;

typedef struct pilhaHash {
    EntradaHash *topo;
    int tamanho_tabela;
    int quantidade_atual;
    struct PilhaHash *resto;
    VariavelSemTipoLst *variaveis_sem_tipo;
    ArgumentoFuncaoLst *argumentos_sem_funcao;
} PilhaHash;

int e4_check_flag;

EntradaHash *_declara_em_escopo(NaturezaSimbolo natureza, TipoSimbolo tipo, ValorLexico valor_lexico, int tamanho_vetor);
EntradaHash *_busca_pilha(char *chave);
EntradaHash *_busca_topo_pilha(char *chave, PilhaHash *pilha);
EntradaHash *_insere_topo_pilha(char *chave, PilhaHash *pilha, Conteudo conteudo);
EntradaHash *_malloc_tabela();
PilhaHash *_malloc_expande_tabela(PilhaHash *pilha);
char *_chave(ValorLexico valor_lexico);
char *_chave_label(char *label);
unsigned long _indice_hash(char *chave);
int _probing(int indice, int capacidade_hash);
int _tamanho(ValorLexico valor_lexico, TipoSimbolo tipo, int tamanho_vetor);
void _libera_tabela();
void _libera_argumentos(ArgumentoFuncaoLst *argumento);
void _libera_vsts(VariavelSemTipoLst *vst);
void _libera_head_vst(VariavelSemTipoLst *vst);
void libera_pilha();
char* _tipo_str(TipoSimbolo tipo);
char* _natureza_str(NaturezaSimbolo natureza);
void _print_tabela(EntradaHash *tabela, int tamanho_tabela);

void print_pilha();
void insere_literal_pilha(TipoSimbolo tipo, ValorLexico valor_lexico);
void insere_funcao_pilha(TipoSimbolo tipo, ValorLexico valor_lexico);
void insere_variavel_pilha(TipoSimbolo tipo, ValorLexico valor_lexico, int tamanho_vetor);

void empilha();
void desempilha();

void insere_variavel_sem_tipo_pilha(ValorLexico valor_lexico);
void insere_vetor_sem_tipo_pilha(ValorLexico valor_lexico, int tamanho_vetor);
void _insere_identificador_sem_tipo_pilha(char *chave, int tamanho_vetor);
void insere_tipo_identificador_pilha(TipoSimbolo tipo);
void atribuicao_simbolo(EntradaHash *entrada1, EntradaHash *entrada2);
ValorLexico _malloc_copia_vlex(ValorLexico valor_lexico);
int _conta_tabelas(PilhaHash *pilha, int count);
Conteudo _novo_conteudo(ValorLexico valor_lexico, Tipo tipo, NaturezaSimbolo natureza, int tamanho_vetor);
Conteudo _novo_conteudo_literal(ValorLexico valor_lexico, Tipo tipo);
EntradaHash *_declara_literal_em_escopo(TipoSimbolo tipo, ValorLexico valor_lexico);
void _print_argumentos(ArgumentoFuncaoLst *argLst);
void insere_argumento_sem_funcao(TipoSimbolo tipo, ValorLexico valor_lexico);
void _verifica_conversao_str(TipoSimbolo tipo, EntradaHash *entrada);
void _verifica_conversao_implicita(Tipo tipo_esq, ValorLexico esq, Tipo tipo_dir, ValorLexico dir, int inicializacao);
void verifica_shift(ValorLexico valor_lexico_int);
ArgumentoFuncaoLst* reverse_args(ArgumentoFuncaoLst* head);
void libera_ultima_funcao();
