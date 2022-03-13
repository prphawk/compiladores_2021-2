#include "ast.h"

nodo *novo_nodo(nodo *pai, valorLexico valor_lexico, nodo **filhos, int num_filhos)
{
    nodo *nodo;

    nodo = malloc(1,sizeof(nodo));
    nodo->valor_lexico = valor_lexico;
    nodo->filhos = filhos;
    nodo->num_filhos = num_filhos;

    return nodo;
}