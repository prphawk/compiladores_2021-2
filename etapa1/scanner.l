%option noyywrap
%{ 
#include<stdio.h>
#include "tokens.h"
%}
DIGIT [0-9]
CHAR [a-zA-Z_]
%%
%%
int main() {
   yylex();
   return 0;
};
