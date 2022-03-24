
#include "ast.h"
#include "valor_lexico.h"
#include <stdio.h>
#include <stdlib.h>

nodo *adiciona_nodo(valorLexico valor_lexico)
{
    nodo *umnodo;
    int extra = 0;

    if(valor_lexico.tipo_literal == STRING)
        extra = strlen((char*)valor_lexico.valor.valor_string)*sizeof(char);
    umnodo = malloc(extra+sizeof(nodo));
    umnodo->valor_lexico = valor_lexico;
    if(valor_lexico.tipo_literal == STRING)
        umnodo->valor_lexico.valor.valor_string = strdup(valor_lexico.valor.valor_string);
    umnodo->filhos = NULL;

    return umnodo;
}

nodo *adiciona_nodo_label(char *label)
{
    valorLexico valor_lexico;
    valor_lexico.linha = -1;
    valor_lexico.tipo = OUTRO;
    valor_lexico.tipo_literal = NAO_LITERAL;
    valor_lexico.label = strdup(label);

    nodo *umnodo;
    umnodo = malloc(sizeof(nodo));

    umnodo->valor_lexico = valor_lexico;
    umnodo->filhos = NULL;

    return umnodo;
}

void adiciona_filho(nodo *pai, nodo *filho) 
{
   if(pai!= NULL && filho!=NULL)
   {
       lseNodo *lse_nodo;
       lse_nodo = malloc(sizeof(lseNodo));

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

void imprime_arvore(nodo *umnodo, int profundidade)
{
    int i = 0;

    if (umnodo == NULL)
        return;
    
    for(i = 0; i<profundidade-1; i++) 
    {
        printf("    ");
    }

    if (profundidade == 0)
        printf("%s", umnodo->valor_lexico.label);
    else 
    {
        printf("+---");
        printf("%s", umnodo->valor_lexico.label);
    }
    printf("\n");

    lseNodo *nodo_f;
    nodo_f = umnodo->filhos;
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

void libera_nodo(nodo *umnodo)
{
    valorLexico valor_lexico = umnodo->valor_lexico;
    free(umnodo);
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

void imprime_nodo(nodo *umnodo)
{
    if (umnodo == NULL)
        return;
    printf("%p [label=\"", umnodo);
    printf("%s", umnodo->valor_lexico.label);
    printf("\"];\n");

    lseNodo *nodo_f;
    nodo_f = umnodo->filhos;
    while(nodo_f!=NULL)
    {
        imprime_nodo(nodo_f->nodo);
        nodo_f = nodo_f->proximo;
    }
    
    return;
}

void imprime_arestas(nodo *umnodo)
{
    if (umnodo == NULL)
        return;

    lseNodo *nodo_f;
    nodo_f = umnodo->filhos;
    while(nodo_f!=NULL)
    {
        printf("%p, %p\n", umnodo, nodo_f);
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