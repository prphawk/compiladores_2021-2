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
void adiciona_filho(nodo *pai, nodo *filho);
lseNodo *acha_ultimo_filho(lseNodo *filhos);
void adiciona_irmao(lseNodo *irmao, lseNodo *novo_irmao);