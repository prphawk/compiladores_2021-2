%{
#include<stdio.h>
int yylex(void);
int yyerror (char const *s); //mudar pra void?
extern int get_line_number (void);
%}
%define parse.error verbose
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

/*The relative precedence of different operators is controlled by the order in which they are declared. 
The first precedence/associativity declaration in the file declares the operators whose precedence is lowest, 
the next ssauch declaration declares the operators whose precedence is a little higher, and so on. 

//https://www.gnu.org/software/bison/manual/html_node/Precedence-Decl.html

Na Sec 3.5 temos os operadores unários, binários e ternários. 
São operadores aritméticos unários: 
+ sinal positivo explı́cito; - inverte o sinal; & acesso ao endereço da variável; * acesso ao valor do ponteiro; 
# acesso a um identificador como uma tabela hash. São operadores aritméticos binários: + soma; - subtração; * multiplicação; / divisão; 
% resto da divisão inteira; | bitwise OR; & bitwise AND; ^ exponenciação. 
São operadores aritméticos ternários: ?: conforme a descrição no texto. 
São operadores lógicos unários e binários: aqueles que sobraram na listagem.
*/
%%

programa: declaracoes;

declaracoes: declaracao declaracoes | ;

declaracao: declaracao_variavel_global | declaracao_funcao;

declaracao_variavel_global: TK_PR_STATIC tipo lista_nome_variavel ';' | tipo lista_nome_variavel ';';

nome_variavel: TK_IDENTIFICADOR | TK_IDENTIFICADOR '[' TK_LIT_INT ']';

lista_nome_variavel: nome_variavel | nome_variavel ',' lista_nome_variavel;

declaracao_funcao: cabecalho corpo;

cabecalho: TK_PR_STATIC tipo TK_IDENTIFICADOR '(' parametros ')' | tipo TK_IDENTIFICADOR '(' parametros ')';

parametros: lista_parametros | ;

lista_parametros: parametro | parametro ',' lista_parametros;

parametro: tipo TK_IDENTIFICADOR | TK_PR_CONST tipo TK_IDENTIFICADOR;

tipo: TK_PR_INT | TK_PR_FLOAT | TK_PR_CHAR | TK_PR_BOOL | TK_PR_STRING;

corpo: bloco_comandos;

lista_comandos: comando_simples ';' lista_comandos | ;

bloco_comandos: '{' lista_comandos '}' ;

chamada_funcao: TK_IDENTIFICADOR'('lista_argumentos')'

comando_simples: declaracao_var_local 
               | comando_atribuicao 
               | comando_entrada 
               | comando_saida 
               | comando_shift 
               | comando_retorno
               | TK_PR_BREAK
               | TK_PR_CONTINUE
               | comando_condicional 
               | comando_iterativo
               | chamada_funcao
               | bloco_comandos
               ;

declaracao_var_local: tipo lista_nome_variavel_local
                     | TK_PR_STATIC TK_PR_CONST tipo lista_nome_variavel_local
                     | TK_PR_CONST tipo lista_nome_variavel_local
                     | TK_PR_STATIC tipo lista_nome_variavel_local
                     ;

lista_nome_variavel_local: cabeca_lista_nome_variavel_local ',' lista_nome_variavel_local | cabeca_lista_nome_variavel_local;
                           
cabeca_lista_nome_variavel_local: TK_IDENTIFICADOR TK_OC_LE TK_IDENTIFICADOR
                                 | TK_IDENTIFICADOR TK_OC_LE literal
                                 | TK_IDENTIFICADOR
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

operador_binario_prec1: '^';
operador_binario_prec2: '*' | '/' | '%';
operador_binario_prec3: '+' | '-';
operador_binario_prec4: '&' | '|';
operador_binario_prec5: '<' | '>' | TK_OC_LE | TK_OC_EQ | TK_OC_GE | TK_OC_NE | operador_binario_logico;

operador_asterisco: '*'

operador_unario: '-' 
               | '+' 
               | '!' 
               | '&' 
               | '?' 
               | '#'
               ;

expressao: expr_ternaria 
         | expr_bin_aritmetica 
         | expr_bin_logica
         ;

expr_ternaria: expr_bin_aritmetica '?' expressao ':' expressao 
               | expr_bin_logica '?' expressao ':' expressao
               ;
            
operando_logico: TK_LIT_TRUE
               | TK_LIT_FALSE
               ;

operador_binario_logico: TK_OC_OR | TK_OC_AND;

operando_aritmetico: TK_IDENTIFICADOR 
               | TK_IDENTIFICADOR'['expr_bin_aritmetica']' 
               | chamada_funcao
               | TK_LIT_FLOAT 
               | TK_LIT_INT
               ;

expr_bin_aritmetica: expr_bin_aritmetica_1 | expr_bin_aritmetica operador_binario_prec5 expr_bin_aritmetica_1;
expr_bin_aritmetica_1: expr_bin_aritmetica_2 | expr_bin_aritmetica_1 operador_binario_prec4 expr_bin_aritmetica_2;
expr_bin_aritmetica_2: expr_bin_aritmetica_3 | expr_bin_aritmetica_2 operador_binario_prec3 expr_bin_aritmetica_3;
expr_bin_aritmetica_3: expr_bin_aritmetica_4 | expr_bin_aritmetica_3 operador_binario_prec2 expr_bin_aritmetica_4;
expr_bin_aritmetica_4: expr_unaria_aritmetica | expr_bin_aritmetica_4 operador_binario_prec1 expr_unaria_aritmetica;

expr_unaria_aritmetica: expr_parenteses_aritmetica | operador_unario expr_parenteses_aritmetica | operador_asterisco expr_unaria_aritmetica;
expr_parenteses_aritmetica: operando_aritmetico | '(' expr_bin_aritmetica ')';
               
expr_bin_logica: expr_bin_logica operador_binario_logico expr_parenteses_logica
               | expr_parenteses_logica operador_binario_logico expr_parenteses_logica;

expr_parenteses_logica: operando_logico | '(' expr_bin_logica ')';

%%
int yyerror (char const *s) {
   printf("line %d: %s\n", get_line_number(), s);
   return 1;
}
