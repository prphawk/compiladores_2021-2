
#include "ast.h"
#include "valor_lexico.h"
#include <stdio.h>
#include <stdlib.h>

Nodo *adiciona_nodo(valorLexico valor_lexico)
{
    Nodo *nodo;
    int extra = 0;

    if(valor_lexico.tipo_literal == STRING)
        extra = strlen((char*)valor_lexico.valor.valor_string)*sizeof(char);
    nodo = malloc(extra+sizeof(Nodo));
    nodo->valor_lexico = valor_lexico;
    if(valor_lexico.tipo_literal == STRING)
        nodo->valor_lexico.valor.valor_string = strdup(valor_lexico.valor.valor_string);
    nodo->filho = NULL;

    return nodo;
}

Nodo *adiciona_nodo_label(char *label)
{
    valorLexico valor_lexico;
    valor_lexico.linha = -1;
    valor_lexico.tipo = OUTRO;
    valor_lexico.tipo_literal = NAO_LITERAL;
    valor_lexico.label = strdup(label);

    Nodo *nodo;
    nodo = malloc(sizeof(Nodo));

    nodo->valor_lexico = valor_lexico;
    nodo->filho = NULL;

    return nodo;
}

void adiciona_filho(Nodo *nodo, Nodo *filho) 
{
   if(nodo!= NULL && filho!=NULL)
   {
       if(nodo->filho == NULL)
       {
           nodo->filho = filho;
       }
       else
       {
           adiciona_irmao(acha_ultimo_irmao(nodo->filho), filho);
       }
   }
   return;
}

void imprime_arvore(Nodo *nodo, int profundidade)
{
    int i = 0;

    if (nodo == NULL)
        return;
    
    for(i = 0; i<profundidade-1; i++) 
    {
        printf("    ");
    }

    if (profundidade == 0)
        printf("%s", nodo->valor_lexico.label);
    else 
    {
        printf("+---");
        printf("%s", nodo->valor_lexico.label);
    }
    printf("\n");

    Nodo *nodo_f = nodo->filho;
    while(nodo_f!=NULL)
    {
        imprime_arvore(nodo_f, profundidade+1);
        nodo_f = nodo_f->irmao;
    }
    
    return;
}

Nodo *acha_ultimo_irmao(Nodo *nodo_irmao)
{
    Nodo *aux_nodo = nodo_irmao;

    while(aux_nodo->irmao!=NULL)
    {
        aux_nodo = aux_nodo->irmao;
    }
    return aux_nodo;
}


void adiciona_irmao(Nodo *nodo, Nodo *novo_irmao)
{
    nodo->irmao = novo_irmao;
    novo_irmao->irmao = NULL;
    return;
}

void libera(void *pai)
{
    if(pai == NULL) return;

    Nodo *pai_arvore = (Nodo*)pai;

    libera(pai_arvore->filho);

    libera(pai_arvore->irmao);

    libera_valor_lexico(pai_arvore->valor_lexico);

    free(pai_arvore);
}

void imprime_nodo(Nodo *nodo)
{
    if (nodo == NULL)
        return;
    printf("%p [label=\"", nodo);
    printf("%s", nodo->valor_lexico.label);
    printf("\"];\n");

    Nodo *nodo_f;
    nodo_f = nodo->filho;
    while(nodo_f!=NULL)
    {
        imprime_nodo(nodo_f);
        nodo_f = nodo_f->irmao;
    }
    
    return;
}

void imprime_arestas(Nodo *nodo)
{
    if (nodo == NULL)
        return;

    Nodo *nodo_f;
    nodo_f = nodo->filho;
    while(nodo_f!=NULL)
    {
        printf("%p, %p\n", nodo, nodo_f);
        imprime_arestas(nodo_f);
        nodo_f = nodo_f->irmao;
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