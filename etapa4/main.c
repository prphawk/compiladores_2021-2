/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "tabela_simbolos.h"

int print_arvore = 1;

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);

int main (int argc, char **argv)
{
  int ret = yyparse(); 
  //exporta (arvore);
  if(print_arvore) imprime_arvore(arvore, 0);
  libera(arvore);
  libera_pilha();
  arvore = NULL;
  yylex_destroy();
  return ret;
}
