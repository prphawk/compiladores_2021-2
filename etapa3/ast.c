#include "ast.h"

nodo *adiciona_nodo(valorLexico valor_lexico)
{
    nodo *nodo;

    nodo = malloc(1,sizeof(nodo));
    nodo->valor_lexico = valor_lexico;
    nodo->filhos = NULL;

    return nodo;
}

void adiciona_filho(nodo *pai, nodo *filho) 
{
    lseNodo lse_nodo;
    lse_nodo.nodo = filho;
    lse_nodo.proximo = NULL;

    if(pai->filhos == NULL)
        pai->filhos = &lse_nodo;
    else
    {
        adiciona_irmao(acha_ultimo_filho(pai->filhos), &lse_nodo);
    }

    return;
}

lseNodo *acha_ultimo_filho(lseNodo *filhos)
{
    lseNodo *aux_nodo = filhos;
    while(aux_nodo->proximo!=NULL)
    {
        aux_nodo = aux_nodo->proximo;
    }
    return aux_nodo;
}

void adiciona_irmao(lseNodo *irmao, lseNodo *novo_irmao)
{
    irmao->proximo = novo_irmao;
    return;
}

void libera_arvore(void *pai)
{
    if(pai == NULL) return;

    nodo *pai_arvore = (nodo*)pai;

    lseNodo *filhos = pai_arvore->filhos;

    free(pai);

    libera_nodo(pai_arvore);
    libera_irmaos(filhos);
    return;
}

void libera_irmaos(void *filhos)
{
    if(filhos == NULL) return;

    lseNodo *irmaos = (nodo*)filhos; 

    nodo *nodo_irmao = irmaos->nodo;
    lseNodo *proximo = irmaos->proximo;

    free(irmaos);

    libera_nodo(nodo_irmao);
    libera_irmaos(proximo);
    return;
}

void libera_nodo(nodo *nodo)
{
    valorLexico valor_lexico = nodo->valor_lexico;
    free(nodo);
    libera_valor_lexico(valor_lexico);
    return;
}

void libera_valor_lexico(valorLexico valor_lexico)
{
    if(valor_lexico.valor.valor_string != NULL)
        free(valor_lexico.valor.valor_string);
    return;
}