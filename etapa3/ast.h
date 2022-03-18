#pragma once
#include <stdio.h>
#include "valor_lexico.h"

typedef struct LseNodo lseNodo;
typedef struct Nodo nodo;

struct Nodo {
    nodo *pai;
    lseNodo *filhos;
    valorLexico valor_lexico;
};

struct LseNodo
{
    nodo *nodo;
    lseNodo *proximo;
};

nodo *adiciona_nodo(valorLexico valor_lexico);
nodo *adiciona_nodo_label(char *label);
void adiciona_filho(nodo *pai, nodo *filho);
void imprime_arvore(nodo *nodo, int profundidade);
lseNodo *acha_ultimo_filho(lseNodo *filhos);
void adiciona_irmao(lseNodo *irmao, lseNodo *novo_irmao);
void libera(void *pai);
void libera_irmaos(void *filhos);
void libera_nodo(nodo *nodo);
void libera_valor_lexico(valorLexico valor_lexico);
void imprime_nodo(nodo *nodo);
void imprime_arestas(nodo *nodo);
void exporta(void *arvore);
