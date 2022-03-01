%{
#include<stdio.h>
int yylex(void);
int yyerror (char const *s); //mudar pra void?
extern int get_line_number (void);

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

//https://www.gnu.org/software/bison/manual/html_node/Precedence-Decl.html
%left TK_OC_LE TK_OC_EQ TK_OC_GE TK_OC_NE TK_OC_OR TK_OC_AND TK_OC_SL TK_OC_SR
%left '+' '-' 
%left '%' '^' '|' '?' ':' '!'
%left '*' '/'
%right '#' '&'
// e os unarios associativos `a direita ???
/*
Na Sec 3.5 temos os operadores unários, binários e ternários. 
São operadores aritméticos unários: 
+ sinal positivo explı́cito; - inverte o sinal; & acesso ao endereço da variável; * acesso ao valor do ponteiro; 
# acesso a um identificador como uma tabela hash. São operadores aritméticos binários: + soma; - subtração; * multiplicação; / divisão; 
% resto da divisão inteira; | bitwise OR; & bitwise AND; ^ exponenciação. 
São operadores aritméticos ternários: ?: conforme a descrição no texto. 
São operadores lógicos unários e binários: aqueles que sobraram na listagem.
*/
%%

/*
programa: declaracoes;

declaracoes: declaracao declaracoes | ;

declaracao: declaracao_variavel_global | declaracao_funcao;

declaracao_variavel_global: static_opcional tipo lista_nome_variavel ';'
                           | tipo lista_nome_variavel ';'
                           ;
*/

nome_variavel: TK_IDENTIFICADOR | TK_IDENTIFICADOR '[' TK_LIT_INT ']'

lista_nome_variavel: nome_variavel | nome_variavel ',' lista_nome_variavel

declaracao_funcao: cabecalho corpo;

cabecalho: static_opcional tipo TK_IDENTIFICADOR '(' parametros ')';

parametros: lista_parametros | ;

lista_parametros: parametro | parametro ',' lista_parametros;

parametro: tipo TK_IDENTIFICADOR | TK_PR_CONST tipo TK_IDENTIFICADOR;

tipo: TK_PR_INT | TK_PR_FLOAT | TK_PR_CHAR | TK_PR_BOOL | TK_PR_STRING;

static_opcional: TK_PR_STATIC | ;

const_opcional: TK_PR_CONST | ;

corpo: bloco_comandos;

bloco_comandos: '{' comando_simples ';' bloco_comandos '}' | ;

comando_simples: declaracao_var_local 
               | comando_atribuicao 
               | comando_entrada 
               | comando_saida 
               //| chamada_funcao 
               | comando_shift 
               | comando_retorno
               | TK_PR_BREAK
               | TK_PR_CONTINUE
               | comando_condicional 
               | comando_iterativo
               | expressao
               | bloco_comandos
               ;

declaracao_var_local: static_opcional const_opcional tipo lista_nome_variavel_local;

lista_nome_variavel_local: TK_IDENTIFICADOR 
                           | TK_IDENTIFICADOR TK_OC_LE TK_IDENTIFICADOR 
                           | TK_IDENTIFICADOR TK_OC_LE literal 
                           | TK_IDENTIFICADOR ',' lista_nome_variavel_local
                           ;

comando_atribuicao: TK_IDENTIFICADOR '=' expressao
                     | TK_IDENTIFICADOR'['expressao']' '=' expressao
                     ;

comando_entrada: TK_PR_INPUT TK_IDENTIFICADOR;

comando_saida: TK_PR_OUTPUT TK_IDENTIFICADOR 
               | TK_PR_OUTPUT literal
               ;



comando_shift: TK_IDENTIFICADOR TK_OC_SL TK_LIT_INT 
               | TK_IDENTIFICADOR'['expressao']' TK_OC_SL TK_LIT_INT
               | TK_IDENTIFICADOR TK_OC_SR TK_LIT_INT 
               | TK_IDENTIFICADOR'['expressao']' TK_OC_SR TK_LIT_INT
               ;

comando_retorno: TK_PR_RETURN expressao;

comando_condicional: TK_PR_IF '(' expressao ')' bloco_comandos
                     | TK_PR_IF '(' expressao ')' bloco_comandos TK_PR_ELSE bloco_comandos
                     ;

comando_iterativo: TK_PR_FOR '(' comando_atribuicao':' expressao':' comando_atribuicao')' bloco_comandos
                  | TK_PR_WHILE '('expressao')' TK_PR_DO bloco_comandos
                  ;


argumento: expressao; 

argumentos: argumento',' argumentos | argumento;

lista_argumentos: argumentos | ;

literal: TK_LIT_CHAR
         | TK_LIT_STRING
         | TK_LIT_TRUE
         | TK_LIT_FALSE
         | TK_LIT_FLOAT 
         | TK_LIT_INT
         ;

expressao: literal
         | '(' expressao ')'
         | TK_IDENTIFICADOR
         | TK_IDENTIFICADOR'[' expressao ']' //vetor
         | TK_IDENTIFICADOR '(' lista_argumentos ')'; // chamada função
         | expressao '+' expressao
         | expressao '-' expressao
         | expressao '%' expressao
         | expressao '*' expressao
         | expressao '/' expressao
         | expressao '|' expressao
         | expressao '&' expressao
         | expressao '^' expressao
         | expressao TK_OC_LE expressao  
         | expressao TK_OC_EQ expressao   
         | expressao TK_OC_GE expressao   
         | expressao TK_OC_NE expressao 
         | expressao TK_OC_OR expressao
	      | expressao TK_OC_AND expressao
         | expressao '?' expressao ':' expressao
         | '+' expressao //DIFERENCIAR OS UNÁRIOS??????
         | '-' expressao
         | '!' expressao
         | '&' expressao
         | '*' expressao
         | '?' expressao
         | '#' expressao 
         /*The relative precedence of different operators is controlled by the order in which they are declared. 
         The first precedence/associativity declaration in the file declares the operators whose precedence is lowest, 
         the next such declaration declares the operators whose precedence is a little higher, and so on. 
         */
         ;

%%

int yyerror (char const *s) {
   printf("%s on line %d\n", s, get_line_number());
   return 1;
}
