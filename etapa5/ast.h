#pragma once
#include <stdio.h>
#include <string.h>
#include "valor_lexico.h"
#include "iloc.h"
typedef enum tipo
{
    TIPO_INT,
    TIPO_FLOAT,
    TIPO_BOOL,
    TIPO_CHAR,
    TIPO_STRING,
    TIPO_OUTRO,
} Tipo;

typedef struct Nodo {
    Tipo tipo;
    ValorLexico valor_lexico;
    struct Nodo *irmao;
    struct Nodo *filho;

    CodigoILOC *codigo;
    //char *resultado;
    Operacao operacao;
} Nodo;

Nodo *adiciona_nodo(ValorLexico valor_lexico);
Nodo *adiciona_nodo_label(char *label);
Nodo *adiciona_nodo_label_concat(char *label, char *label2);
void adiciona_filho(Nodo *nodo, Nodo *filho);
void _imprime_arvore(Nodo *nodo, int profundidade);
Nodo *_acha_ultimo_irmao(Nodo *nodo_irmao);
void adiciona_irmao(Nodo *nodo, Nodo *novo_irmao);
void _adiciona_ultimo_irmao(Nodo *irmao, Nodo *novo_irmao);


void _exporta (void *arvore);
void _libera(void *pai);
void _imprime_label_nodo(Nodo *nodo);
void _imprime_arestas(Nodo *nodo);
int possui_tipo(Tipo t1, Tipo t2, Tipo tipo);
int possui_tipo_nodo(Nodo *nodo1, Nodo *nodo2, Tipo tipo);
Tipo get_tipo_inferencia(Nodo *nodo1, Nodo *nodo2);
char* _get_label_nodo(Nodo *nodo);
void _imprime_filhos(Nodo *nodo);

void print_arvore();
void exporta_arvore();
void libera_arvore();