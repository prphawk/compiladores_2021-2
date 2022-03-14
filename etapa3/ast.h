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