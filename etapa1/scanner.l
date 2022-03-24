%option noyywrap
%x comment
%{ 
#include<stdio.h>
#include "tokens.h"
void succ_num_lines();
int get_ascii_value(char* yytext);

int num_lines = 1;

int get_line_number(void) { return num_lines; }
%}
DIGIT [0-9]
ALPHA_ [a-zA-Z_]
ID {ALPHA_}({ALPHA_}|{DIGIT})*
SPECIAL [,;:()\[\]{}\+\-|\*\/<>=!&?%#\^.$]
INT {DIGIT}+ 
FLOAT {DIGIT}+\.{DIGIT}+((E|e){INT})?
CHAR '.'
STRING \"([^\n"])*\"
COMMENT_SINGLE \/\/[^\n\r]*
TO_IGNORE [ \t\r]+|{COMMENT_SINGLE}
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
<comment>"\n" succ_num_lines();
<comment>[^*\n]* /* precisamos dessa (consumir tudo que não for "*") e a pattern abaixo para que tudo
que for escrito num comentário não fechado seja consumido e não interpretado como nada além de erro */
<comment>"*"+[^\/\n]* /* consome "*" não seguido de "/" */
<comment><<EOF>> { BEGIN(INITIAL); return TOKEN_ERRO;};
<comment>"*"+"/" BEGIN(INITIAL);

{SPECIAL} return get_ascii_value(yytext);
. return TOKEN_ERRO;
%%
int get_ascii_value(char* yytext) { return (int) yytext[0]; }
void succ_num_lines() { ++num_lines; }