
#include "ast.h"
#include "valor_lexico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tipo get_tipo_nodo(ValorLexico valor_lexico) {
    if(valor_lexico.tipo_vlex == VLEX_TIPO_LITERAL) {
        switch (valor_lexico.tipo_vlex_literal) {
            case VLEX_LITERAL_INT: return TIPO_INT; break;
            case VLEX_LITERAL_FLOAT: return TIPO_FLOAT; break;
            case VLEX_LITERAL_BOOL: return TIPO_BOOL; break;
            case VLEX_LITERAL_CHAR: return TIPO_CHAR; break;
            case VLEX_LITERAL_STRING: return TIPO_STRING; break;
        }
    }
    return TIPO_OUTRO;
}

Tipo get_tipo_inferencia(Nodo *nodo1, Nodo *nodo2) {

    if(nodo1->tipo == nodo2->tipo) return nodo1->tipo;

    if(possui_tipo(nodo1, nodo2, TIPO_INT)) {
        if(possui_tipo(nodo1, nodo2, TIPO_FLOAT))
            return TIPO_FLOAT;
        if(possui_tipo(nodo1, nodo2, TIPO_BOOL))
            return TIPO_INT;
    }

    if(possui_tipo(nodo1, nodo2, TIPO_FLOAT) 
    && possui_tipo(nodo1, nodo2, TIPO_BOOL))
        return TIPO_FLOAT;

    return TIPO_OUTRO;
}

int possui_tipo(Nodo *nodo1, Nodo *nodo2, Tipo tipo) {
    return (nodo1->tipo == tipo || nodo2->tipo == tipo);
}

Nodo *adiciona_nodo(ValorLexico valor_lexico)
{
    Nodo *nodo;
    nodo = malloc(sizeof(Nodo));

    nodo->filho = NULL;
    nodo->irmao = NULL;
    nodo->valor_lexico = valor_lexico;
    nodo->tipo = get_tipo_nodo(valor_lexico);

    return nodo;
}

Nodo *adiciona_nodo_label_concat(char *label, char *label2) {
    char* result = append_str_malloc(label, label2);
    Nodo *nodo = adiciona_nodo_label(result);
    free(result);
    return nodo;
}

Nodo *adiciona_nodo_label(char *label)
{
    ValorLexico valor_lexico;
    valor_lexico.linha = -1; //TODO deveriamos fazer algo sobre isso?
    valor_lexico.tipo_vlex = VLEX_TIPO_OUTRO;
    valor_lexico.tipo_vlex_literal = VLEX_LITERAL_NULL;
    valor_lexico.label = strdup(label);
    valor_lexico.valor_string = NULL;

    Nodo *nodo;
    nodo = malloc(sizeof(Nodo));

    nodo->valor_lexico = valor_lexico;
    nodo->filho = NULL;
    nodo->irmao = NULL;
    nodo->tipo = TIPO_OUTRO;

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
        printf("%s (%s)", nodo->valor_lexico.label, _tipo_str(nodo->tipo));
    else 
    {
        printf("●---");
        printf("%s (%s)", nodo->valor_lexico.label, _tipo_str(nodo->tipo));
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

    libera_vlex(pai_arvore->valor_lexico);

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