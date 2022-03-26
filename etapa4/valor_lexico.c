#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "valor_lexico.h"

ValorLexico atribui_yylval(char* yytext, TipoVLex tipo, TipoVLexLiteral tipo_vlex_literal, int num_lines) 
{
      ValorLexico valor_lexico;
      valor_lexico.linha=num_lines;
      valor_lexico.tipo_vlex=tipo;
      valor_lexico.tipo_vlex_literal=tipo_vlex_literal;
      valor_lexico.label = strdup(yytext);

      switch(tipo_vlex_literal) {
         case VLEX_LITERAL_BOOL:
            valor_lexico.valor_bool = (strncmp (yytext,"true",4) == 0); //if Return value = 0 then it indicates str1 is equal to str2.
            break;
         case VLEX_LITERAL_FLOAT:
            valor_lexico.valor_float=atof(yytext);
            break;
         case VLEX_LITERAL_INTEIRO:
            valor_lexico.valor_int=atoi(yytext);
            break;
         case VLEX_LITERAL_CHAR:
            valor_lexico.valor_char=yytext[1];
            break;
         case VLEX_LITERAL_STRING:
            valor_lexico.valor_string = strdup(yytext+1);
            valor_lexico.valor_string[strlen(valor_lexico.valor_string)-1] = '\0';
            break;
      }

      return valor_lexico;
}

void libera_valor_lexico(ValorLexico valor_lexico)
{
    if(tem_valor_string(valor_lexico))
        free(valor_lexico.valor_string);
    if(valor_lexico.label != NULL)
        free(valor_lexico.label);
    return;
}

int tem_valor_string(ValorLexico valor_lexico) {
   return (valor_lexico.tipo_vlex_literal == VLEX_LITERAL_STRING);
}