#pragma once
#include <stdio.h>
#include <string.h>
#include "valor_lexico.h"

typedef struct Nodo {
    struct Nodo *pai;
    struct LseNodo *filhos;
    valorLexico valor_lexico;
    // TODO modificar função de adicionar nodo para incluir o tipo
    // TipoSimbolo tipo;
} Nodo;

typedef struct LseNodo
{
    Nodo *valor_nodo;
    struct LseNodo *proximo;
} LseNodo;

Nodo *adiciona_nodo(valorLexico valor_lexico);
Nodo *adiciona_nodo_label(char *label);
void adiciona_filho(Nodo *pai, Nodo *filho);
void imprime_arvore(Nodo *umnodo, int profundidade);
LseNodo *acha_ultimo_filho(LseNodo *filhos);
void adiciona_irmao(LseNodo *irmao, LseNodo *novo_irmao);
void libera(void *pai);
void libera_irmaos(void *filhos);
void libera_nodo(Nodo *umnodo);
void libera_valor_lexico(valorLexico valor_lexico);
void imprime_nodo(Nodo *umnodo);
void imprime_arestas(Nodo *umnodo);
void exporta(void *arvore);
