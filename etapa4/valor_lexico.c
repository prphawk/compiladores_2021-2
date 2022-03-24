#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "valor_lexico.h"

valorLexico atribui_yylval(char* yytext, Tipo tipo, TipoLiteral tipo_literal, int num_lines) 
{
    valorLexico valor_lexico;
    valor_lexico.linha=num_lines; 
    valor_lexico.tipo=tipo; 
    valor_lexico.tipo_literal=tipo_literal;
    valor_lexico.label = strdup(yytext);

    switch(tipo_literal) {
      case NAO_LITERAL:
         valor_lexico.valor.valor_string = strdup(yytext);
         break;
      case BOOL:
         valor_lexico.valor.valor_bool = (strncmp (yytext,"true",4) == 0); //conversao de int pra bool implicita; if Return value = 0 then it indicates str1 is equal to str2.
         break;
      case FLOAT:
         valor_lexico.valor.valor_float=atof(yytext);
         break;
      case INTEIRO:
         valor_lexico.valor.valor_int=atoi(yytext);
         break;
      case CHAR:
         valor_lexico.valor.valor_char=yytext[1]; // TODO [1] mesmo?
         break;
      case STRING:
         valor_lexico.valor.valor_string = strdup(yytext);
         valor_lexico.valor.valor_string++;
         valor_lexico.valor.valor_string[strlen(valor_lexico.valor.valor_string)-1] = '\0';
         break;
    }

    return valor_lexico;
}