#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "valor_lexico.h"

valorLexico atribui_yylval(char* yytext, Tipo tipo, TipoLiteral tipo_literal, int num_lines) 
{
      valorLexico valor_lexico;
      valor_lexico.linha=num_lines;
      valor_lexico.tipo=tipo;
      valor_lexico.tipo_literal=tipo_literal;
      valor_lexico.label = strdup(yytext);

      switch(tipo_literal) {
         case BOOL:
            valor_lexico.valor.valor_bool = (strncmp (yytext,"true",4) == 0); //if Return value = 0 then it indicates str1 is equal to str2.
            break;
         case FLOAT:
            valor_lexico.valor.valor_float=atof(yytext);
            break;
         case INTEIRO:
            valor_lexico.valor.valor_int=atoi(yytext);
            break;
         case CHAR:
            valor_lexico.valor.valor_char=strdup(yytext[1]);
            break;
         case STRING:
            valor_lexico.valor.valor_string = strdup(yytext+1);
            valor_lexico.valor.valor_string[strlen(valor_lexico.valor.valor_string)-1] = '\0';
            break;
      }


      return valor_lexico;
}

void libera_valor_lexico(valorLexico valor_lexico)
{
    if(tem_valor_string(valor_lexico))
        free(valor_lexico.valor.valor_string);
    if(valor_lexico.label != NULL)
        free(valor_lexico.label);
    return;
}

int tem_valor_string(valorLexico valor_lexico) {
   return (valor_lexico.tipo_literal == STRING || valor_lexico.tipo_literal == CHAR);
}