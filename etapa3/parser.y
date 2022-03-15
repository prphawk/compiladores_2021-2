%{
#include<stdio.h>
#include "ast.h"
int yylex(void);
int yyerror (char const *s); //mudar pra void?
extern int get_line_number (void);
nodo *adiciona_nodo(valorLexico valor_lexico);
void adiciona_filho(nodo *pai, nodo *filho) ;
lseNodo *acha_ultimo_filho(lseNodo *filhos);
void adiciona_irmao(lseNodo *irmao, lseNodo *novo_irmao);
void libera_arvore(void *pai);
void libera_irmaos(void *filhos);
void libera_nodo(nodo *nodo);
void libera_valor_lexico(valorLexico valor_lexico);
%}
%code requires {
   #include "valor_lexico.h"
}
%union {
   valorLexico valor_lexico;
   nodo *nodo;
}
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
%token<valor_lexico> TK_OC_LE
%token<valor_lexico> TK_OC_GE
%token<valor_lexico> TK_OC_EQ
%token<valor_lexico> TK_OC_NE
%token<valor_lexico> TK_OC_AND
%token<valor_lexico> TK_OC_OR
%token<valor_lexico> TK_OC_SL
%token<valor_lexico> TK_OC_SR
%token<valor_lexico> TK_LIT_INT
%token<valor_lexico> TK_LIT_FLOAT
%token<valor_lexico> TK_LIT_FALSE
%token<valor_lexico> TK_LIT_TRUE
%token<valor_lexico> TK_LIT_CHAR
%token<valor_lexico> TK_LIT_STRING
%token<valor_lexico> TK_IDENTIFICADOR
%token TOKEN_ERRO

%type<nodo> literal
%type<nodo> lista_nome_variavel_local

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
               | expressao
               | bloco_comandos
               ;

declaracao_var_local: TK_PR_STATIC TK_PR_CONST tipo lista_nome_variavel_local
                     | TK_PR_CONST tipo lista_nome_variavel_local
                     | TK_PR_STATIC tipo lista_nome_variavel_local
                     | tipo lista_nome_variavel_local
                     ;

lista_nome_variavel_local: TK_IDENTIFICADOR { $$ = adiciona_nodo($1);}
                           | TK_IDENTIFICADOR TK_OC_LE TK_IDENTIFICADOR 
                            { 
                                nodo *novo_nodo = adiciona_nodo($2);
                                adiciona_filho(novo_nodo, adiciona_nodo($1));
                                adiciona_filho(novo_nodo, adiciona_nodo($3));
                                $$ = novo_nodo;
                            }
                           | TK_IDENTIFICADOR TK_OC_LE literal 
                           { 
                                nodo *novo_nodo = adiciona_nodo($2);
                                adiciona_filho(novo_nodo, adiciona_nodo($1));
                                adiciona_filho(novo_nodo, $3);
                                $$ = novo_nodo;
                            }
                           | TK_IDENTIFICADOR ',' lista_nome_variavel_local
                           { 
                                adiciona_filho($$, adiciona_nodo($1));
                                adiciona_filho($$, $3);
                            }
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

literal: TK_LIT_CHAR { $$ = adiciona_nodo($1);}
         | TK_LIT_STRING { $$ = adiciona_nodo($1);}
         | TK_LIT_TRUE { $$ = adiciona_nodo($1);}
         | TK_LIT_FALSE { $$ = adiciona_nodo($1);}
         | TK_LIT_FLOAT { $$ = adiciona_nodo($1);}
         | TK_LIT_INT { $$ = adiciona_nodo($1);}
         ;

operador_binario: '*'
                  | '/'
                  | '^'
                  | '+'
                  | '-'
                  | '%'
                  | '|'
                  | '&'
                  | '<'
                  | '>'
                  | TK_OC_LE
                  | TK_OC_EQ   
                  | TK_OC_GE   
                  | TK_OC_NE 
                  | TK_OC_OR
                  | TK_OC_AND
                  ;

operador_unario: '-' 
               | '+' 
               | '!' 
               | '&' 
               | '*' 
               | '?' 
               | '#'
               ;

expressao: expr_binaria_ou | expressao '?' expr_binaria_ou ':' expr_binaria_ou;
expr_binaria_ou: expr_unaria_ou | expr_binaria_ou operador_binario expr_unaria_ou;
expr_unaria_ou: expr_parenteses_ou | operador_unario expr_parenteses_ou;
expr_parenteses_ou: operando | '(' expressao ')'; 

operando: TK_IDENTIFICADOR 
         | TK_IDENTIFICADOR'['expressao']' 
         | TK_IDENTIFICADOR'('lista_argumentos')'
         | TK_LIT_TRUE
         | TK_LIT_FALSE
         | TK_LIT_FLOAT 
         | TK_LIT_INT
         ; 
%%
int yyerror (char const *s) {
   printf("line %d: %s\n", get_line_number(), s);
   return 1;
}

nodo *adiciona_nodo(valorLexico valor_lexico)
{
    nodo *nodo;

    nodo = malloc(sizeof(nodo));
    nodo->valor_lexico = valor_lexico;
    nodo->filhos = NULL;

    return nodo;
}

void adiciona_filho(nodo *pai, nodo *filho) 
{

   lseNodo lse_nodo;
   lse_nodo.nodo = filho;
   lse_nodo.proximo = NULL;
   lse_nodo.nodo->pai = pai;

   if(pai->filhos == NULL)
   {
      pai->filhos = &lse_nodo;
   }
   else
   {
      adiciona_irmao(acha_ultimo_filho(pai->filhos), &lse_nodo);
   }

   return;
}

//TODO função de alterar nodo
//TODO função de remover nodo
//TODO função de printar árvore

lseNodo *acha_ultimo_filho(lseNodo *filhos)
{
    lseNodo *aux_nodo = filhos;
    while(aux_nodo->proximo!=NULL)
    {
        aux_nodo = aux_nodo->proximo;
    }
    return aux_nodo;
}

void adiciona_irmao(lseNodo *irmao, lseNodo *novo_irmao)
{
    irmao->proximo = novo_irmao;
    return;
}

void libera_arvore(void *pai)
{
    if(pai == NULL) return;

    nodo *pai_arvore = (nodo*)pai;

    lseNodo *filhos = pai_arvore->filhos;

    free(pai);

    libera_nodo(pai_arvore);
    libera_irmaos(filhos);
    return;
}

void libera_irmaos(void *filhos)
{
    if(filhos == NULL) return;

    lseNodo *irmaos = (lseNodo*)filhos; 

    nodo *nodo_irmao = irmaos->nodo;
    lseNodo *proximo = irmaos->proximo;

    free(irmaos);

    libera_nodo(nodo_irmao);
    libera_irmaos(proximo);
    return;
}

void libera_nodo(nodo *nodo)
{
    valorLexico valor_lexico = nodo->valor_lexico;
    free(nodo);
    libera_valor_lexico(valor_lexico);
    return;
}

void libera_valor_lexico(valorLexico valor_lexico)
{
    if(valor_lexico.valor.valor_string != NULL)
        free(valor_lexico.valor.valor_string);
    return;
}