#pragma once
#include <stdio.h>
#include <string.h>
#include "valor_lexico.h"

typedef enum tipo
{
    INTEIRO,
    FLOAT,
    BOOL,
    CHAR,
    STRING,
    NAO_DEFINIDO
} Tipo;


typedef struct Nodo {
    Tipo tipo;
    valorLexico valor_lexico;
    struct Nodo *irmao;
    struct Nodo *filho;
} Nodo;

Nodo *adiciona_nodo(valorLexico valor_lexico);
Nodo *adiciona_nodo_label(char *label);
Nodo *adiciona_nodo_label_concat(char *label, valorLexico valor_lexico);
void adiciona_filho(Nodo *nodo, Nodo *filho);
void imprime_arvore(Nodo *nodo, int profundidade);
Nodo *acha_ultimo_irmao(Nodo *nodo_irmao);
void adiciona_irmao(Nodo *irmao, Nodo *novo_irmao);
void libera(void *pai);
void libera_irmaos(void *filhos);
void libera_nodo(Nodo *nodo);
void imprime_nodo(Nodo *nodo);
void imprime_arestas(Nodo *nodo);
void exporta(void *arvore);
