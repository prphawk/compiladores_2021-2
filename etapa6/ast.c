
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *arvore = NULL;

extern char*_tipo_str(Tipo tipo);
extern void codigo_finaliza(Nodo *arvore);
extern void generateAsm(Nodo *arvore);

Tipo _get_tipo_cod(ValorLexico valor_lexico) {
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

    if(possui_tipo_cod(nodo1, nodo2, TIPO_INT)) {
        if(possui_tipo_cod(nodo1, nodo2, TIPO_FLOAT))
            return TIPO_FLOAT;
        if(possui_tipo_cod(nodo1, nodo2, TIPO_BOOL))
            return TIPO_INT;
    }

    if(possui_tipo_cod(nodo1, nodo2, TIPO_FLOAT) 
    && possui_tipo_cod(nodo1, nodo2, TIPO_BOOL))
        return TIPO_FLOAT;

    return TIPO_OUTRO;
}

int possui_tipo_cod(Nodo *nodo1, Nodo *nodo2, Tipo tipo) {
    return (nodo1->tipo == tipo || nodo2->tipo == tipo);
}

int possui_tipo(Tipo t1, Tipo t2, Tipo tipo) {
    return (t1 == tipo || t2 == tipo);
}

Nodo *adiciona_nodo(ValorLexico valor_lexico)
{
    Nodo *nodo;
    nodo = malloc(sizeof(Nodo));

    nodo->filho = NULL;
    nodo->irmao = NULL;
    nodo->valor_lexico = valor_lexico;
    nodo->tipo = _get_tipo_cod(valor_lexico);

    nodo->codigo = NULL;
    nodo->reg_resultado = NULL;
    nodo->tipo_cod = cod_null;

    nodo->remendos_false = NULL;
    nodo->remendos_true = NULL;
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
    valor_lexico.linha = -1;
    valor_lexico.tipo_vlex = VLEX_TIPO_OUTRO;
    valor_lexico.tipo_vlex_literal = VLEX_LITERAL_NULL;
    valor_lexico.label = strdup(label);
    valor_lexico.valor_string = NULL;

    return adiciona_nodo(valor_lexico);
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
           _adiciona_ultimo_irmao(_acha_ultimo_irmao(nodo->filho), filho);
       }
   }
}

void _imprime_arvore(Nodo *nodo, int profundidade)
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
        _imprime_arvore(nodo_f, profundidade+1);
        nodo_f = nodo_f->irmao;
    }
}

Nodo *_acha_ultimo_irmao(Nodo *nodo_irmao)
{
    Nodo *aux_nodo = nodo_irmao;

    if(aux_nodo == NULL) return NULL;

    while(aux_nodo->irmao!=NULL)
    {
        aux_nodo = aux_nodo->irmao;
    }
    return aux_nodo;
}


void _adiciona_ultimo_irmao(Nodo *nodo, Nodo *novo_irmao)
{
    adiciona_irmao(nodo, novo_irmao);
    novo_irmao->irmao = NULL;
}

void adiciona_irmao(Nodo *nodo, Nodo *novo_irmao)
{
    nodo->irmao = novo_irmao;
}

void libera_arvore() {
    _libera(arvore);
    arvore = NULL;
}

void exporta_arvore() {
    _exporta(arvore);
}

void print_arvore() {
    _imprime_arvore(arvore, 0);
}

void _libera(void *pai)
{
    if(pai == NULL) return;

    Nodo *pai_arvore = pai;

    _libera(pai_arvore->filho);

    _libera(pai_arvore->irmao);

    libera_vlex(pai_arvore->valor_lexico);

    libera_codigo(pai_arvore->codigo);

    free(pai_arvore);
}

char* _get_label_nodo(Nodo *nodo) {
    if(nodo->valor_lexico.tipo_vlex_literal == VLEX_LITERAL_STRING)
        return nodo->valor_lexico.valor_string; 
     
    return nodo->valor_lexico.label;
}

void _imprime_label_nodo(Nodo *nodo)
{
    printf("%p [label=\"", nodo);
    printf("%s", _get_label_nodo(nodo));
    printf("\"];\n");
}

void _exporta(void *arvore)
{
    Nodo *nodo = arvore;
   
    if (nodo == NULL) return;

    _imprime_label_nodo(nodo);

    _imprime_filhos(nodo);

    Nodo *nodo_f = nodo->filho;

    while(nodo_f!=NULL)
    {
        _exporta(nodo_f);
        nodo_f = nodo_f->irmao;
    }
}

void _imprime_filhos(Nodo *nodo) {

    Nodo *nodo_f = nodo->filho;

    while(nodo_f != NULL) {

        printf("%p, %p\n", nodo, nodo_f);

        nodo_f = nodo_f->irmao;
    }
}

void finaliza_codigo_ILOC()
{
    codigo_finaliza(arvore);
}

void exporta_codigo_ILOC()
{
    Nodo *root = arvore;
    if(root == NULL) return;
    print_codigo(root->codigo);
}

void exporta_codigo_ASM() {
    generateAsm(arvore);
}