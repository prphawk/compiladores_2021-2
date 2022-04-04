#pragma once
#include <stdio.h>
#include <string.h>
#include "valor_lexico.h"
#include "tabela_simbolos.h"

typedef struct Nodo {
    Tipo tipo;
    ValorLexico valor_lexico;
    struct Nodo *irmao;
    struct Nodo *filho;
} Nodo;

Nodo *adiciona_nodo(ValorLexico valor_lexico);
Nodo *adiciona_nodo_label(char *label);
Nodo *adiciona_nodo_label_concat(char *label, char *label2);
void adiciona_filho(Nodo *nodo, Nodo *filho);
void imprime_arvore(Nodo *nodo, int profundidade);
Nodo *acha_ultimo_irmao(Nodo *nodo_irmao);
void adiciona_irmao(Nodo *nodo, Nodo *novo_irmao);
void adiciona_ultimo_irmao(Nodo *irmao, Nodo *novo_irmao);
void libera(void *pai);
void libera_irmaos(void *filhos);
void libera_nodo(Nodo *nodo);
void imprime_nodo(Nodo *nodo);
void imprime_arestas(Nodo *nodo);
void exporta(void *arvore);
int possui_tipo(Nodo *nodo1, Nodo *nodo2, Tipo tipo);
