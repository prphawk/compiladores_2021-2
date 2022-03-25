
#include "ast.h"
#include "valor_lexico.h"
#include <stdio.h>
#include <stdlib.h>

Nodo *adiciona_nodo(valorLexico valor_lexico)
{
    Nodo *umnodo;
    int extra = 0;

    if(valor_lexico.tipo_literal == STRING)
        extra = strlen((char*)valor_lexico.valor.valor_string)*sizeof(char);
    umnodo = malloc(extra+sizeof(Nodo));
    umnodo->valor_lexico = valor_lexico;
    if(valor_lexico.tipo_literal == STRING)
        umnodo->valor_lexico.valor.valor_string = strdup(valor_lexico.valor.valor_string);
    umnodo->filhos = NULL;

    return umnodo;
}

Nodo *adiciona_nodo_label(char *label)
{
    valorLexico valor_lexico;
    valor_lexico.linha = -1;
    valor_lexico.tipo = OUTRO;
    valor_lexico.tipo_literal = NAO_LITERAL;
    valor_lexico.label = strdup(label);

    Nodo *umnodo;
    umnodo = malloc(sizeof(Nodo));

    umnodo->valor_lexico = valor_lexico;
    umnodo->filhos = NULL;

    return umnodo;
}

void adiciona_filho(Nodo *pai, Nodo *filho) 
{
   if(pai!= NULL && filho!=NULL)
   {
       LseNodo *lse_nodo;
       lse_nodo = malloc(sizeof(LseNodo));

       lse_nodo->valor_nodo = filho;
       lse_nodo->proximo = NULL;
       lse_nodo->valor_nodo->pai = pai;
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

void imprime_arvore(Nodo *umnodo, int profundidade)
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

    Nodo *nodo_f = nodo->filho;
    while(nodo_f!=NULL)
    {
        imprime_arvore(nodo_f->valor_nodo, profundidade+1);
        nodo_f = nodo_f->proximo;
    }
    
    return;
}

LseNodo *acha_ultimo_filho(LseNodo *filhos)
{
    LseNodo *aux_nodo = filhos;
    while(aux_nodo->proximo!=NULL)
    {
        aux_nodo = aux_nodo->proximo;
    }
    return aux_nodo;
}

void adiciona_irmao(LseNodo *irmao, LseNodo *novo_irmao)
{
    irmao->proximo = novo_irmao;
    novo_irmao->proximo = NULL;
    return;
}

void libera(void *pai)
{
    if(pai == NULL) return;

    Nodo *pai_arvore = (Nodo*)pai;

    LseNodo *filhos = pai_arvore->filhos;

    libera_nodo(pai_arvore);
    libera_irmaos(filhos);
    return;
}

void libera_irmaos(void *filhos)
{
    if(filhos == NULL) return;

    LseNodo *irmaos = (LseNodo*)filhos; 

    Nodo *nodo_irmao = irmaos->valor_nodo;
    LseNodo *proximo = irmaos->proximo;
    
    free(irmaos);

    libera(nodo_irmao);
    libera_irmaos(proximo);
    return;
}

void libera_nodo(Nodo *umnodo)
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

void imprime_nodo(Nodo *umnodo)
{
    if (umnodo == NULL)
        return;
    printf("%p [label=\"", umnodo);
    printf("%s", umnodo->valor_lexico.label);
    printf("\"];\n");

    LseNodo *nodo_f;
    nodo_f = umnodo->filhos;
    while(nodo_f!=NULL)
    {
        imprime_nodo(nodo_f->valor_nodo);
        nodo_f = nodo_f->proximo;
    }
    
    return;
}

void imprime_arestas(Nodo *umnodo)
{
    if (umnodo == NULL)
        return;

    LseNodo *nodo_f;
    nodo_f = umnodo->filhos;
    while(nodo_f!=NULL)
    {
        printf("%p, %p\n", umnodo, nodo_f);
        imprime_arestas(nodo_f->valor_nodo);
        nodo_f = nodo_f->proximo;
    }
    
    return;
}

void exporta(void *arvore)
{
    Nodo *nodo_arvore;
    nodo_arvore = (Nodo*) arvore;
    imprime_nodo(nodo_arvore);
    imprime_arestas(nodo_arvore);
    return;
}