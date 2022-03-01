%{
#include<stdio.h>
int yylex(void);
int yyerror (char const *s); //mudar pra void?
%}
%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_PR_CONST
%token TK_PR_STATIC
%token TK_PR_FOREACH
%token TK_PR_FOR
%token TK_PR_SWITCH
%token TK_PR_CASE
%token TK_PR_BREAK
%token TK_PR_CONTINUE
%token TK_PR_CLASS
%token TK_PR_PRIVATE
%token TK_PR_PUBLIC
%token TK_PR_PROTECTED
%token TK_PR_END
%token TK_PR_DEFAULT
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_OC_SL
%token TK_OC_SR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO

%%

programa: declaracoes;

declaracoes: declaracao declaracoes | ;

declaracao: declaracao_variavel | declaracao_funcao;

declaracao_variavel: static_opcional tipo nome_variavel ';'; // aqui tem q ser uma lista pra validar int a, b, c; 
// o static tem q ser opcional

nome_variavel: TK_IDENTIFICADOR | TK_IDENTIFICADOR '[' TK_LIT_INT ']'

static_opcional: TK_PR_STATIC | ;

tipo: TK_PR_INT | TK_PR_FLOAT | TK_PR_CHAR | TK_PR_BOOL | TK_PR_STRING;

declaracao_funcao: cabecalho corpo;

cabecalho: static_opcional tipo TK_IDENTIFICADOR '(' parametros ')';

parametros: lista_parametros | ;

lista_parametros: parametro | parametro ',' lista_parametros;

parametro: tipo TK_IDENTIFICADOR | TK_PR_CONST tipo TK_IDENTIFICADOR;

corpo: bloco_comandos;

bloco_comandos: '{' comando_simples ';' bloco_comandos '}' | ;

comando_simples: declaracao_var 
               | comando_atribuicao 
               | comando_entrada 
               | comando_saida 
               | chamada_funcao 
               | comando_shift 
               | comando_retorno 
               | comando_break 
               | comando_continue 
               | comando_condicional 
               | expressao 
               | bloco_comandos;

declaracao_var: ;

comando_atribuicao: ;

comando_entrada: ;

comando_saida: ;

chamada_funcao: ;

comando_shift: ;

comando_retorno: TK_PR_RETURN expressao ;

comando_break: TK_PR_BREAK;

comando_continue: TK_PR_CONTINUE;

comando_condicional: TK_PR_IF '(' expressao ')' bloco_comandos ;

expressao: ;

%%

int yyerror (char const *s) {
   printf("%s\n", s);
   return 1;
}
