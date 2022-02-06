%option noyywrap
%{ 
#include<stdio.h>
#include "tokens.h"
static void print_regex();
%}
DIGIT [0-9]
CHAR [a-zA-Z_]
%%
{DIGIT} { print_regex(); }
{CHAR} { print_regex(); }
%%
static void print_regex() {
   printf("foi regex");
}
int main() {
   yylex();
   return 0;
};
