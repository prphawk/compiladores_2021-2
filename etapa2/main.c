/*
Função principal para realização da análise sintática.

Este arquivo será posterioremente substituído, não acrescente nada.
*/
#include <stdio.h>
#include "parser.tab.h" //arquivo gerado com bison -d parser.y
extern int yydebug;
extern int yylex_destroy(void);

int main (int argc, char **argv)
{
  //yydebug = 1; //ferramenta para debugar a gramática caso tenha algum conflito (veja o manual do Bison sobre o uso).
  int ret = yyparse();
  yylex_destroy();
  return ret;
}
