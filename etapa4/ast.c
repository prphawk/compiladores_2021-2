
#include "ast.h"
#include "valor_lexico.h"
#include <stdio.h>
#include <stdlib.h>

nodo *adiciona_nodo(valorLexico valor_lexico)
{
    nodo *nodo;
    int extra = 0;

    if(valor_lexico.tipo_literal == STRING)
        extra = strlen((char*)valor_lexico.valor.valor_string)*sizeof(char);
    nodo = malloc(extra+sizeof(nodo)*8);
    nodo->valor_lexico = valor_lexico;
    if(valor_lexico.tipo_literal == STRING)
        nodo->valor_lexico.valor.valor_string = strdup(valor_lexico.valor.valor_string);
    nodo->filhos = NULL;

    return nodo;
}

nodo *adiciona_nodo_label(char *label)
{
    valorLexico valor_lexico;
    valor_lexico.linha = -1;
    valor_lexico.tipo = OUTRO;
    valor_lexico.tipo_literal = NAO_LITERAL;
    valor_lexico.label = strdup(label);

    nodo *nodo;

    nodo = malloc(sizeof(nodo)*8);
    nodo->valor_lexico = valor_lexico;
    nodo->filhos = NULL;

    return nodo;
}

void adiciona_filho(nodo *pai, nodo *filho) 
{
   if(pai!= NULL && filho!=NULL)
   {
       lseNodo *lse_nodo;
       lse_nodo = malloc(sizeof(lse_nodo)*8);
       lse_nodo->nodo = filho;
       lse_nodo->proximo = NULL;
       lse_nodo->nodo->pai = pai;
       if(pai->filhos == NULL)
       {
           pai->filhos = lse_nodo;
       }
       else
       {
           adiciona_irmao(acha_ultimo_filho(pai->filhos), lse_nodo);
       }
   }
   return;
   
}
//TODO função de alterar nodo
//TODO função de remover nodo
void imprime_arvore(nodo *nodo, int profundidade)
{
    int i = 0;

    if (nodo == NULL)
        return;
    
    for(i = 0; i<profundidade-1; i++) 
    {
        printf("    ");
    }

    if (profundidade == 0)
        printf(nodo->valor_lexico.label);
    else 
    {
        printf("+---");
        printf(nodo->valor_lexico.label);
    }
    printf("\n");

    lseNodo *nodo_f;
    nodo_f = nodo->filhos;
    while(nodo_f!=NULL)
    {
        imprime_arvore(nodo_f->nodo, profundidade+1);
        nodo_f = nodo_f->proximo;
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
    novo_irmao->proximo = NULL;
    return;
}

void libera(void *pai)
{
    if(pai == NULL) return;

    nodo *pai_arvore = (nodo*)pai;

    lseNodo *filhos = pai_arvore->filhos;

    libera_nodo(pai_arvore);
    libera_irmaos(filhos);
    return;
}

void libera_irmaos(void *filhos)
{
    if(filhos == NULL) return;

    lseNodo *irmaos = (lseNodo*)filhos; 

    nodo *nodo_irmao = irmaos->nodo;
    lseNodo *proximo = irmaos->proximo;
    
    free(irmaos);

    libera(nodo_irmao);
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
    if(valor_lexico.tipo_literal == STRING)
        free(valor_lexico.valor.valor_string);
    if(valor_lexico.label != NULL)
        free(valor_lexico.label);
    return;
}

void imprime_nodo(nodo *nodo)
{
    if (nodo == NULL)
        return;
    printf("%p [label=\"", nodo);
    printf(nodo->valor_lexico.label);
    printf("\"];\n");

    lseNodo *nodo_f;
    nodo_f = nodo->filhos;
    while(nodo_f!=NULL)
    {
        imprime_nodo(nodo_f->nodo);
        nodo_f = nodo_f->proximo;
    }
    
    return;
}

void imprime_arestas(nodo *nodo)
{
    if (nodo == NULL)
        return;

    lseNodo *nodo_f;
    nodo_f = nodo->filhos;
    while(nodo_f!=NULL)
    {
        printf("%p, %p\n", nodo, nodo_f);
        imprime_arestas(nodo_f->nodo);
        nodo_f = nodo_f->proximo;
    }
    
    return;
}

void exporta(void *arvore)
{
    nodo *nodo_arvore;
    nodo_arvore = (nodo*) arvore;
    imprime_nodo(nodo_arvore);
    imprime_arestas(nodo_arvore);
    return;
}