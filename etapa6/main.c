#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "tabela_simbolos.h"
#include "mem.h"

int print_simbolos_global = 0;
int print_ILOC_intermed_global = 0;

extern int yyparse(void);
extern int yylex_destroy(void);

int main (int argc, char **argv)
{
  int ret = yyparse(); 
  //exporta_arvore();
  //print_arvore();
  finaliza_codigo_ILOC();
  exporta_codigo_ILOC();
  exporta_codigo_ASM();
  free_mem();
  yylex_destroy();
  return ret;
}
