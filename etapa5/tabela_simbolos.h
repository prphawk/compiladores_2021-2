#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "valor_lexico.h"
#include "errors.h"
#include "ast.h"

#define TAMANHO_INICIAL_HASH 389
#define TAMANHO_CHAR 1 
#define TAMANHO_INT 4
#define TAMANHO_FLOAT 8

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

typedef struct ArgumentoFuncaoLst
{
    TipoSimbolo tipo;
    struct ArgumentoFuncaoLst *proximo;
} ArgumentoFuncaoLst;

typedef struct VariavelSemTipoLst
{
    char *chave;
    int tamanho_vetor;
    struct VariavelSemTipoLst *proximo;
} VariavelSemTipoLst;

typedef struct conteudo {
    int linha;
    int tamanho;
    TipoSimbolo tipo;
    NaturezaSimbolo natureza;
    ArgumentoFuncaoLst* argumentos;
    ValorLexico valor_lexico;
} Conteudo;

typedef struct entradaHash
{
    char *chave;
    int deslocamento;
    Conteudo conteudo;
} EntradaHash;

typedef struct DeslocamentoEscopo
{
    int deslocamento;
    int eh_escopo_global;
} DeslocamentoEscopo;

typedef struct PilhaHash {
    EntradaHash *topo;
    int tamanho_tabela;
    int quantidade_atual;
    int deslocamento;
    struct PilhaHash *resto;
} PilhaHash;

int _eh_escopo_global(PilhaHash *pilha);
int _conta_tabelas(PilhaHash *pilha, int count);
int _conta_argumentos(ArgumentoFuncaoLst *args);
int _conta_argumentos_nodo(Nodo *nodo);
int _tamanho(ValorLexico valor_lexico, TipoSimbolo tipo, int tamanho_vetor);
char *_chave(ValorLexico valor_lexico);
char *_chave_label(char *label);
unsigned long _indice_hash(char *chave);
int _probing(int indice, int capacidade_hash);
void _malloc_expande_tabela(PilhaHash *pilha);
Conteudo _novo_conteudo(ValorLexico valor_lexico, Tipo tipo, NaturezaSimbolo natureza, int tamanho_vetor);
Conteudo _novo_conteudo_literal(ValorLexico valor_lexico, Tipo tipo);
EntradaHash *_busca_pilha(char *chave);
EntradaHash *_busca_topo_pilha(char *chave, PilhaHash *pilha);

void insere_literal_pilha(TipoSimbolo tipo, ValorLexico valor_lexico);
void insere_variavel_pilha(TipoSimbolo tipo, ValorLexico valor_lexico, int tamanho_vetor);
void insere_funcao_pilha(TipoSimbolo tipo, ValorLexico valor_lexico);
void insere_variavel_sem_tipo_pilha(ValorLexico valor_lexico);
void insere_vetor_sem_tipo_pilha(ValorLexico valor_lexico, int tamanho_vetor);
void _adiciona_identificador_sem_tipo(char *chave, int tamanho_vetor);
void insere_tipo_identificador_pilha(TipoSimbolo tipo);
void insere_parametro_sem_funcao(TipoSimbolo tipo, ValorLexico valor_lexico);
void adiciona_parametros_escopo_anterior(Nodo *nodo_funcao);
ArgumentoFuncaoLst* reverse_args(ArgumentoFuncaoLst* head);
ValorLexico _malloc_copia_vlex(ValorLexico valor_lexico);
void _atualiza_deslocamento_topo(int tamanho);
EntradaHash *_declara_em_escopo(NaturezaSimbolo natureza, TipoSimbolo tipo, ValorLexico valor_lexico, int tamanho_vetor);
EntradaHash *_declara_literal_em_escopo(TipoSimbolo tipo, ValorLexico valor_lexico);
EntradaHash *_insere_topo_pilha(char *chave, PilhaHash *pilha, Conteudo conteudo);
void empilha();
EntradaHash *_malloc_tabela();
void _inicializa_entrada(EntradaHash *entrada);

void libera_pilha();
void desempilha();
void _libera_tabela();
void libera_ultima_funcao();
void _libera_head_vst(VariavelSemTipoLst *vst);
void _libera_vsts();
void _libera_argumentos(ArgumentoFuncaoLst *argumento);

void verifica_atribuicao(Nodo *esq, Nodo *operador, Nodo *dir);
void verifica_return(Nodo *operador, Nodo *expr1);
void verifica_expr_ternaria(Nodo *validacao, Nodo *expr1, Nodo *expr2, Nodo *operador);
void verifica_expr_binaria(Nodo *esq, Nodo *operador, Nodo *dir);
void verifica_expr_unaria(Nodo *nodo_unario, Nodo *nodo);
void verifica_input(Nodo *nodo_input, Nodo *nodo);
void _verifica_op_str_char_erro(Nodo *nodo, ValorLexico vlex_op);
void verifica_output(Nodo *nodo_output, Nodo *nodo);
void verifica_shift(Nodo *nodo_shift, ValorLexico valor_lexico_int);
void verifica_inicializacao(Tipo tipo, Nodo *nodos_inicializados);
void _verifica_parametros_funcao(ArgumentoFuncaoLst *parametros, EntradaHash *entrada_funcao);
void _verifica_conversao_implicita(Tipo tipo_esq, ValorLexico esq, Tipo tipo_dir, ValorLexico dir, int inicializacao);
void _verifica_tamanho_maximo_string(Tipo tipo_dir, ValorLexico esq, ValorLexico dir, int inicializacao);
void _verifica_vetor_string(TipoSimbolo tipo, EntradaHash *entrada);
void verifica_variavel_no_escopo(Nodo *nodo);
void verifica_vetor_no_escopo(Nodo *nodo_vetor, Nodo *nodo_identificador);
void verifica_funcao_no_escopo(ValorLexico valor_lexico, Nodo *args_passados, Nodo *chamada_funcao);
void _verifica_tipos_argumentos(Nodo *args_passados, ArgumentoFuncaoLst *args_declarados, int linha_declarada);

void print_pilha();
void _print_tabela(EntradaHash *tabela, int tamanho_tabela);
char* eh_escopo_global_str(PilhaHash *pilha);
char* _tipo_str(TipoSimbolo tipo);
char* _natureza_str(NaturezaSimbolo natureza);
void _print_argumentos(ArgumentoFuncaoLst *argLst);

DeslocamentoEscopo busca_deslocamento_e_escopo(char *chave);