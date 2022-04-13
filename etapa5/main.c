/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "tabela_simbolos.h"
#include "mem.h"

int print_simbolos = 0;

extern int yyparse(void);
extern int yylex_destroy(void);

int main (int argc, char **argv)
{
  int ret = yyparse(); 
  //exporta_arvore();
  //print_arvore();
  //imprime_codigo_ILOC();
  free_mem();
  yylex_destroy();
  return ret;
}
