#pragma once
#include <stdio.h>
#include <string.h>
#include "valor_lexico.h"

typedef struct Nodo {
    valorLexico valor_lexico;
    struct Nodo *irmao;
    struct Nodo *filho;
    // TODO modificar função de adicionar nodo para incluir o tipo
    // TipoSimbolo tipo;
} Nodo;

Nodo *adiciona_nodo(valorLexico valor_lexico);
Nodo *adiciona_nodo_label(char *label);
void adiciona_filho(Nodo *nodo, Nodo *filho);
void imprime_arvore(Nodo *nodo, int profundidade);
Nodo *acha_ultimo_irmao(Nodo *nodo_irmao);
void adiciona_irmao(Nodo *irmao, Nodo *novo_irmao);
void libera(void *pai);
void libera_irmaos(void *filhos);
void libera_nodo(Nodo *nodo);
void libera_valor_lexico(valorLexico valor_lexico);
void imprime_nodo(Nodo *nodo);
void imprime_arestas(Nodo *nodo);
void exporta(void *arvore);
