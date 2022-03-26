
#include "ast.h"
#include "valor_lexico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Nodo *adiciona_nodo(valorLexico valor_lexico)
{
    Nodo *nodo;
    nodo = malloc(sizeof(Nodo));

    nodo->filho = NULL;
    nodo->irmao = NULL;
    nodo->valor_lexico = valor_lexico;

    return nodo;
}

Nodo *adiciona_nodo_label_concat(char *label, valorLexico valor_lexico) {
    char* str2 = valor_lexico.label;
    char* result = malloc((strlen(label)+strlen(str2)+1)*sizeof(char));
    strcpy(result, label); strcat(result, str2);
    Nodo *nodo = adiciona_nodo_label(result);
    free(result);
    return nodo;
}

Nodo *adiciona_nodo_label(char *label)
{
    valorLexico valor_lexico;
    valor_lexico.linha = -1; //deveriamos fazer algo sobre isso?
    valor_lexico.tipo_vlex = VLEX_TIPO_OUTRO;
    valor_lexico.tipo_vlex_literal = VLEX_LITERAL_NULL;
    valor_lexico.label = strdup(label);
    valor_lexico.valor_string = NULL;

    Nodo *nodo;
    nodo = malloc(sizeof(Nodo));

    nodo->valor_lexico = valor_lexico;
    nodo->filho = NULL;
    nodo->irmao = NULL;

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