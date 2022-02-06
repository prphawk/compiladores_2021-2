%option noyywrap
%x comment
%{ 
#include<stdio.h>
#include "tokens.h"
#include "main.c"
void succ_num_lines();
int get_ascii_value(char* yytext);

int num_lines = 1;

int get_line_number(void) { return num_lines; }
%}
DIGIT [0-9]
ALPHA_ [a-zA-Z_]
ID {ALPHA_}({ALPHA_}|{DIGIT})*
SPECIAL [,;:()\[\]{}\+\-|\*\/<>=!&%#\^.$]
OPT_SIGNAL (\+|\-)?
INT {OPT_SIGNAL}{DIGIT}+ 
FLOAT ({OPT_SIGNAL}{DIGIT}*)?\.{DIGIT}+((E|e){INT})?
CHAR '.'
STRING \"(\\.|[^\n"\\])*\"
/* eh impressao minha ou ele n botou a especificação de string no doc mas deixou o token?? 

\\. -> https://stringr.tidyverse.org/articles/regular-expressions.html#escaping
https://stackoverflow.com/questions/2039795/regular-expression-for-a-string-literal-in-flex-lex
*/
COMMENT_SINGLE \/\/[^\n\r]*
/*n sei se boto o \n opcional no final ou não (e se tiver um  no final, sem \n e ele n identificar?), meus testes foram inconclusivos*/
TO_IGNORE [ \t\n]+|{COMMENT_SINGLE}
%%
\n succ_num_lines();
"int" return TK_PR_INT;
"float" return TK_PR_FLOAT;
"bool" return TK_PR_BOOL;
"char" return TK_PR_CHAR;
"string" return TK_PR_STRING;
"if" return TK_PR_IF;
"then" return TK_PR_THEN;
"else" return TK_PR_ELSE;
"while" return TK_PR_WHILE;
"do" return TK_PR_DO;
"input" return TK_PR_INPUT;
"output" return TK_PR_OUTPUT;
"return" return TK_PR_RETURN;
"const" return TK_PR_CONST;
"static" return TK_PR_STATIC;
"foreach" return TK_PR_FOREACH;
"for" return TK_PR_FOR;
"switch" return TK_PR_SWITCH;
"case" return TK_PR_CASE;
"break" return TK_PR_BREAK;
"continue" return TK_PR_CONTINUE;
"class" return TK_PR_CLASS;
"private" return TK_PR_PRIVATE;
"public" return TK_PR_PUBLIC;
"protected" return TK_PR_PROTECTED;
"end" return TK_PR_END;
"default" return TK_PR_DEFAULT;

"<=" return TK_OC_LE;
">=" return TK_OC_GE;
"==" return TK_OC_EQ;
"!=" return TK_OC_NE;
"&&" return TK_OC_AND;
"||" return TK_OC_OR; 
">>" return TK_OC_SL;
"<<" return TK_OC_SR;

"true" return TK_LIT_TRUE;
"false" return TK_LIT_FALSE;

{ID} return TK_IDENTIFICADOR;
{FLOAT} return TK_LIT_FLOAT;
{INT} return TK_LIT_INT;
{CHAR} return TK_LIT_CHAR;
{STRING} return TK_LIT_STRING;
{TO_IGNORE}

"/*" BEGIN(comment);
<comment>[^*\n]*
<comment>"*"+[^*/\n]*
<comment>"\n" succ_num_lines();
<comment>"*"+"/" BEGIN(INITIAL); /* de http://westes.github.io/flex/manual/Start-Conditions.html*/

{SPECIAL} return get_ascii_value(yytext);
.+ return TOKEN_ERRO;
%%
int get_ascii_value(char* yytext) { return (int) yytext[0]; }
void succ_num_lines() { ++num_lines; }