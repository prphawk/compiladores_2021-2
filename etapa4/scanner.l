%option noyywrap
%x comment
%{ 
#include<stdio.h>
#include<string.h>
#include "ast.h"
#include "parser.tab.h"
#define YYSTYPE valorLexico

void succ_num_lines();
int get_ascii_value(char* yytext);
void atribui_yylval(Tipo tipo, TipoLiteral tipo_literal) ;
void atribui_string();

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

">=" { atribui_yylval(OPERADOR_COMPOSTO, NAO_LITERAL); yylval.valor_lexico.valor.valor_string = strdup(yytext); return TK_OC_GE;}
"<=" { atribui_yylval(OPERADOR_COMPOSTO, NAO_LITERAL); yylval.valor_lexico.valor.valor_string = strdup(yytext); return TK_OC_LE;}
"==" { atribui_yylval(OPERADOR_COMPOSTO, NAO_LITERAL); yylval.valor_lexico.valor.valor_string = strdup(yytext); return TK_OC_EQ;}
"!=" { atribui_yylval(OPERADOR_COMPOSTO, NAO_LITERAL); yylval.valor_lexico.valor.valor_string = strdup(yytext); return TK_OC_NE;}
"&&" { atribui_yylval(OPERADOR_COMPOSTO, NAO_LITERAL); yylval.valor_lexico.valor.valor_string = strdup(yytext); return TK_OC_AND;}
"||" { atribui_yylval(OPERADOR_COMPOSTO, NAO_LITERAL); yylval.valor_lexico.valor.valor_string = strdup(yytext); return TK_OC_OR;}
">>" { atribui_yylval(OPERADOR_COMPOSTO, NAO_LITERAL); yylval.valor_lexico.valor.valor_string = strdup(yytext); return TK_OC_SL;}
"<<" { atribui_yylval(OPERADOR_COMPOSTO, NAO_LITERAL); yylval.valor_lexico.valor.valor_string = strdup(yytext); return TK_OC_SR;}

"true" { atribui_yylval(LITERAL, BOOL); yylval.valor_lexico.valor.valor_bool=1; return TK_LIT_TRUE;}
"false" { atribui_yylval(LITERAL, BOOL); yylval.valor_lexico.valor.valor_bool=0; return TK_LIT_FALSE;}

{ID} { atribui_yylval(IDENTIFICADOR, NAO_LITERAL); yylval.valor_lexico.valor.valor_string = strdup(yytext); return TK_IDENTIFICADOR;}
{FLOAT} { atribui_yylval(LITERAL, FLOAT); yylval.valor_lexico.valor.valor_int=atof(yytext); return TK_LIT_FLOAT;}
{INT} { atribui_yylval(LITERAL, INTEIRO); yylval.valor_lexico.valor.valor_int=atoi(yytext); return TK_LIT_INT;}
{CHAR} { atribui_yylval(LITERAL, CHAR); yylval.valor_lexico.valor.valor_char=yytext[1]; return TK_LIT_CHAR;}
{STRING} { atribui_yylval(LITERAL, STRING); atribui_string(); return TK_LIT_STRING;}
{TO_IGNORE}

"/*" BEGIN(comment);
<comment>"\n" succ_num_lines();
<comment>[^*\n]* /* precisamos dessa (consumir tudo que não for "*") e a pattern abaixo para que tudo
que for escrito num comentário não fechado seja consumido e não interpretado como nada além de erro */
<comment>"*"+[^\/\n]* /* consome "*" não seguido de "/" */
<comment><<EOF>> { BEGIN(INITIAL); return TOKEN_ERRO;};
<comment>"*"+"/" BEGIN(INITIAL);

{SPECIAL} {atribui_yylval(CARACTERE_ESPECIAL, NAO_LITERAL); yylval.valor_lexico.valor.valor_string = strdup(yytext); return get_ascii_value(yytext);}
. return TOKEN_ERRO;
%%
int get_ascii_value(char* yytext) { return (int) yytext[0]; }
void succ_num_lines() { ++num_lines; }
void atribui_yylval(Tipo tipo, TipoLiteral tipo_literal) 
{
    yylval.valor_lexico.linha=num_lines; 
    yylval.valor_lexico.tipo=tipo; 
    yylval.valor_lexico.tipo_literal=tipo_literal;
    yylval.valor_lexico.label = strdup(yytext);
}

void atribui_string() 
{
    yylval.valor_lexico.valor.valor_string = strdup(yytext);
    yylval.valor_lexico.valor.valor_string++;
    yylval.valor_lexico.valor.valor_string[strlen(yylval.valor_lexico.valor.valor_string)-1] = '\0';
}