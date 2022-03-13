#include <stdio.h>
#include "valor_lexico.h"

typedef struct nodo
{
    nodo *pai;
    nodo **filhos;
    int num_filhos;
    valorLexico valor_lexico;
} nodo;

nodo *novo_nodo(nodo *pai, valorLexico valor_lexico, nodo **filhos, int num_filhos);
