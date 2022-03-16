%{
#include<stdio.h>
#include<string.h>
#include "ast.h"
#include "main.h"
int yylex(void);
int yyerror (char const *s); //mudar pra void?
extern int get_line_number (void);
nodo *adiciona_nodo(valorLexico valor_lexico);
nodo *adiciona_nodo_label(char *label);
void adiciona_filho(nodo *pai, nodo *filho);
void imprime_arvore(nodo *nodo, int profundidade);
lseNodo *acha_ultimo_filho(lseNodo *filhos);
void adiciona_irmao(lseNodo *irmao, lseNodo *novo_irmao);
void libera_arvore(void *pai);
void libera_irmaos(void *filhos);
void libera_nodo(nodo *nodo);
void libera_valor_lexico(valorLexico valor_lexico);
%}
%define parse.error verbose
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
%type<nodo> declaracao_var_local
%type<nodo> comando_simples
%type<nodo> comando_atribuicao
%type<nodo> comando_entrada
%type<nodo> comando_saida
%type<nodo> comando_shift
%type<nodo> comando_retorno
%type<nodo> comando_condicional
%type<nodo> comando_iterativo
%type<nodo> expressao
%type<nodo> bloco_comandos
%type<nodo> operador_binario_prec1
%type<nodo> operador_binario_prec2
%type<nodo> operador_binario_prec3
%type<nodo> operador_binario_prec4
%type<nodo> operador_binario_prec5
%type<nodo> operador_unario
%type<nodo> operador_asterisco
%type<nodo> expr_binaria_ou
%type<nodo> expr_binaria_1_ou
%type<nodo> expr_binaria_2_ou
%type<nodo> expr_binaria_3_ou
%type<nodo> expr_binaria_4_ou
%type<nodo> expr_unaria_ou
%type<nodo> expr_parenteses_ou
%type<nodo> operando
%type<nodo> lista_comandos
%type<nodo> corpo
%type<nodo> lista_argumentos
%type<nodo> argumentos
%type<nodo> argumento
%type<nodo> cabecalho
%type<nodo> declaracao_funcao
%type<nodo> declaracao
%type<nodo> declaracoes
%type<nodo> programa

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

programa: declaracoes { $$ = $1; arvore = $$; imprime_arvore($1, 0); };

declaracoes: declaracao declaracoes 
            {
                if($2!=NULL) adiciona_filho($1, $2);
                $$ = $1;
            } | { $$ = NULL; };

declaracao: declaracao_variavel_global | declaracao_funcao { $$ = $1; };

declaracao_variavel_global: TK_PR_STATIC tipo lista_nome_variavel ';' | tipo lista_nome_variavel ';';

nome_variavel: TK_IDENTIFICADOR | TK_IDENTIFICADOR '[' TK_LIT_INT ']';

lista_nome_variavel: nome_variavel | nome_variavel ',' lista_nome_variavel;

declaracao_funcao: cabecalho corpo 
                {
                    adiciona_filho($1, $2);
                    $$ = $1;
                };

cabecalho: TK_PR_STATIC tipo TK_IDENTIFICADOR '(' parametros ')' { $$ = adiciona_nodo($3); }
            | tipo TK_IDENTIFICADOR '(' parametros ')' { $$ = adiciona_nodo($2); };

parametros: lista_parametros | ;

lista_parametros: parametro | parametro ',' lista_parametros;

parametro: tipo TK_IDENTIFICADOR | TK_PR_CONST tipo TK_IDENTIFICADOR;

tipo: TK_PR_INT | TK_PR_FLOAT | TK_PR_CHAR | TK_PR_BOOL | TK_PR_STRING;

corpo: bloco_comandos { $$ = $1; } ;

lista_comandos: comando_simples ';' lista_comandos { if($3!=NULL) adiciona_filho($1, $3); $$ = $1; }
                | { $$ = NULL; };

bloco_comandos: '{' lista_comandos '}' { $$ = $2; } ;

comando_simples: declaracao_var_local { $$ = $1;}
               | comando_atribuicao { $$ = $1;}
               | comando_entrada { $$ = $1;}
               | comando_saida { $$ = $1;}
               | comando_shift { $$ = $1;}
               | comando_retorno { $$ = $1;}
               | TK_PR_BREAK { $$ = adiciona_nodo_label("break"); }
               | TK_PR_CONTINUE { $$ = adiciona_nodo_label("continue"); }
               | comando_condicional { $$ = $1;}
               | comando_iterativo { $$ = $1;}
               | expressao { $$ = $1;}
               | bloco_comandos { $$ = $1;}
               ;

declaracao_var_local: TK_PR_STATIC TK_PR_CONST tipo lista_nome_variavel_local { $$ = $4;}
                     | TK_PR_CONST tipo lista_nome_variavel_local { $$ = $3;}
                     | TK_PR_STATIC tipo lista_nome_variavel_local { $$ = $3;}
                     | tipo lista_nome_variavel_local { $$ = $2;}
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
                    {
                        nodo *novo_nodo = adiciona_nodo_label("=");
                        adiciona_filho(novo_nodo, adiciona_nodo($1));
                        adiciona_filho(novo_nodo, $3);
                        $$ = novo_nodo;
                    }
                     | TK_IDENTIFICADOR'['expressao']' '=' expressao
                    {
                        nodo *nodo_vetor = adiciona_nodo_label("[]");
                        adiciona_filho(nodo_vetor, adiciona_nodo($1));
                        adiciona_filho(nodo_vetor, $3);

                        nodo *novo_nodo = adiciona_nodo_label("=");
                        adiciona_filho(novo_nodo, nodo_vetor);
                        adiciona_filho(novo_nodo, $6);
                        $$ = novo_nodo;
                    }
                     ;

comando_entrada: TK_PR_INPUT TK_IDENTIFICADOR
                {
                    nodo *novo_nodo = adiciona_nodo_label("input");
                    adiciona_filho(novo_nodo, adiciona_nodo($2));
                    $$ = novo_nodo;
                };

comando_saida: TK_PR_OUTPUT TK_IDENTIFICADOR 
                {
                    nodo *novo_nodo = adiciona_nodo_label("output");
                    adiciona_filho(novo_nodo, adiciona_nodo($2));
                    $$ = novo_nodo;
                }
               | TK_PR_OUTPUT literal
               {
                    nodo *novo_nodo = adiciona_nodo_label("output");
                    adiciona_filho(novo_nodo, $2);
                    $$ = novo_nodo;
                }
               ;

comando_shift: TK_IDENTIFICADOR TK_OC_SL TK_LIT_INT 
                {
                    nodo *novo_nodo = adiciona_nodo($2);
                    adiciona_filho(novo_nodo, adiciona_nodo($1));
                    adiciona_filho(novo_nodo, adiciona_nodo($3));
                    $$ = novo_nodo;
                }
               | TK_IDENTIFICADOR'['expressao']' TK_OC_SL TK_LIT_INT
               {
                    nodo *nodo_vetor = adiciona_nodo_label("[]");
                    adiciona_filho(nodo_vetor, adiciona_nodo($1));
                    adiciona_filho(nodo_vetor, $3);
                    nodo *novo_nodo = adiciona_nodo($5);
                    adiciona_filho(novo_nodo, nodo_vetor);
                    adiciona_filho(novo_nodo, adiciona_nodo($6));
                    $$ = novo_nodo;
                }
               | TK_IDENTIFICADOR TK_OC_SR TK_LIT_INT 
               {
                    nodo *novo_nodo = adiciona_nodo($2);
                    adiciona_filho(novo_nodo, adiciona_nodo($1));
                    adiciona_filho(novo_nodo, adiciona_nodo($3));
                    $$ = novo_nodo;
                }
               | TK_IDENTIFICADOR'['expressao']' TK_OC_SR TK_LIT_INT
               {
                    nodo *nodo_vetor = adiciona_nodo_label("[]");
                    adiciona_filho(nodo_vetor, adiciona_nodo($1));
                    adiciona_filho(nodo_vetor, $3);
                    nodo *novo_nodo = adiciona_nodo($5);
                    adiciona_filho(novo_nodo, nodo_vetor);
                    adiciona_filho(novo_nodo, adiciona_nodo($6));
                    $$ = novo_nodo;
                }
               ;

comando_retorno: TK_PR_RETURN expressao
                {
                    nodo *novo_nodo = adiciona_nodo_label("return");
                    adiciona_filho(novo_nodo, $2);
                    $$ = novo_nodo;
                };

comando_condicional: TK_PR_IF '(' expressao ')' bloco_comandos 
                        {
                            nodo *novo_nodo = adiciona_nodo_label("if");
                            adiciona_filho(novo_nodo, $3);
                            adiciona_filho(novo_nodo, $5);
                            $$ = novo_nodo;
                        }
                     | TK_PR_IF '(' expressao ')' bloco_comandos TK_PR_ELSE bloco_comandos
                        {
                            nodo *novo_nodo = adiciona_nodo_label("if");
                            adiciona_filho(novo_nodo, $3);
                            adiciona_filho(novo_nodo, $5);
                            adiciona_filho(novo_nodo, $7);
                            $$ = novo_nodo;
                        }
                     ;

comando_iterativo: TK_PR_FOR '(' comando_atribuicao':' expressao':' comando_atribuicao')' bloco_comandos
                    {
                        nodo *novo_nodo = adiciona_nodo_label("for");
                        adiciona_filho(novo_nodo, $3);
                        adiciona_filho(novo_nodo, $5);
                        adiciona_filho(novo_nodo, $7);
                        adiciona_filho(novo_nodo, $9);
                        $$ = novo_nodo;
                    }
                  | TK_PR_WHILE '('expressao')' TK_PR_DO bloco_comandos
                  {
                      nodo *novo_nodo = adiciona_nodo_label("while");
                      adiciona_filho(novo_nodo, $3);
                      adiciona_filho(novo_nodo, $6);
                      $$ = novo_nodo;
                  }
                  ;

argumento: expressao { $$ = $1; };

argumentos: argumento',' argumentos 
            {;
                adiciona_filho($1, $3);
                $$ = $1;
            }
            | argumento { $$ = $1; };

lista_argumentos: argumentos { $$ = $1; }| { $$ = NULL; };

literal: TK_LIT_CHAR { $$ = adiciona_nodo($1);}
         | TK_LIT_STRING { $$ = adiciona_nodo($1);}
         | TK_LIT_TRUE { $$ = adiciona_nodo($1);}
         | TK_LIT_FALSE { $$ = adiciona_nodo($1);}
         | TK_LIT_FLOAT { $$ = adiciona_nodo($1);}
         | TK_LIT_INT { $$ = adiciona_nodo($1);}
         ;

operador_binario_prec1: '^' { $$ = adiciona_nodo_label("^"); };
operador_binario_prec2: '*' { $$ = adiciona_nodo_label("*"); } | '/' { $$ = adiciona_nodo_label("/"); } | '%' { $$ = adiciona_nodo_label("%"); };
operador_binario_prec3: '+' { $$ = adiciona_nodo_label("+"); } | '-' { $$ = adiciona_nodo_label("-"); };
operador_binario_prec4: '&' { $$ = adiciona_nodo_label("&"); } | '|' { $$ = adiciona_nodo_label("|"); };
operador_binario_prec5: '<' { $$ = adiciona_nodo_label("<"); } 
                    | '>' { $$ = adiciona_nodo_label(">"); } 
                    | TK_OC_LE { $$ = adiciona_nodo($1); }
                    | TK_OC_EQ { $$ = adiciona_nodo($1); }
                    | TK_OC_GE { $$ = adiciona_nodo($1); }
                    | TK_OC_NE { $$ = adiciona_nodo($1); }
                    | TK_OC_OR { $$ = adiciona_nodo($1); }
                    | TK_OC_AND { $$ = adiciona_nodo($1); }
                    ;

operador_asterisco: '*' { $$ = adiciona_nodo_label("*"); } 

operador_unario: '-' { $$ = adiciona_nodo_label("-"); }
               | '+' { $$ = adiciona_nodo_label("+"); } 
               | '!' { $$ = adiciona_nodo_label("!"); } 
               | '&' { $$ = adiciona_nodo_label("&"); } 
               | '?' { $$ = adiciona_nodo_label("?"); } 
               | '#' { $$ = adiciona_nodo_label("#"); }
               ;

expressao: expr_binaria_ou { $$ = $1; }
            | expressao '?' expr_binaria_ou ':' expr_binaria_ou
            {
                nodo *novo_nodo = adiciona_nodo_label("?:");
                adiciona_filho(novo_nodo, $1);
                adiciona_filho(novo_nodo, $3);
                adiciona_filho(novo_nodo, $5);
                $$ = novo_nodo;
            };
expr_binaria_ou: expr_binaria_1_ou { $$ = $1; }
                | expr_binaria_ou operador_binario_prec5 expr_binaria_1_ou
                {
                    adiciona_filho($2, $1);
                    adiciona_filho($2, $3);
                    $$ = $2;
                };
expr_binaria_1_ou: expr_binaria_2_ou { $$ = $1; }
                | expr_binaria_1_ou operador_binario_prec4 expr_binaria_2_ou
                {
                    adiciona_filho($2, $1);
                    adiciona_filho($2, $3);
                    $$ = $2;
                };
expr_binaria_2_ou: expr_binaria_3_ou { $$ = $1; }
                | expr_binaria_2_ou operador_binario_prec3 expr_binaria_3_ou
                {
                    adiciona_filho($2, $1);
                    adiciona_filho($2, $3);
                    $$ = $2;
                };
expr_binaria_3_ou: expr_binaria_4_ou { $$ = $1; }
                | expr_binaria_3_ou operador_binario_prec2 expr_binaria_4_ou
                {
                    adiciona_filho($2, $1);
                    adiciona_filho($2, $3);
                    $$ = $2;
                };
expr_binaria_4_ou: expr_unaria_ou { $$ = $1; }
                | expr_binaria_4_ou operador_binario_prec1 expr_unaria_ou
                {
                    adiciona_filho($2, $1);
                    adiciona_filho($2, $3);
                    $$ = $2;
                };            
expr_unaria_ou: expr_parenteses_ou { $$ = $1; }
                | operador_unario expr_parenteses_ou 
                {
                    adiciona_filho($1, $2);
                    $$ = $1;
                }
                | operador_asterisco expr_unaria_ou
                {
                    adiciona_filho($1, $2);
                    $$ = $1;
                };

expr_parenteses_ou: operando { $$ = $1; } 
                    | '(' expressao ')' { $$ = $2; }; 

operando: TK_IDENTIFICADOR { $$ = adiciona_nodo($1); }
         | TK_IDENTIFICADOR'['expressao']' 
        { 
            nodo *novo_nodo = adiciona_nodo_label("[]");
            adiciona_filho(novo_nodo, adiciona_nodo($1));
            adiciona_filho(novo_nodo, $3);
            $$ = novo_nodo;
        }
         | TK_IDENTIFICADOR'('lista_argumentos')' 
        { 
            nodo *novo_nodo = adiciona_nodo_label("call");
            adiciona_filho(novo_nodo, adiciona_nodo($1));
            adiciona_filho(novo_nodo, $3);
            $$ = novo_nodo;
        }
         | TK_LIT_TRUE { $$ = adiciona_nodo($1); }
         | TK_LIT_FALSE { $$ = adiciona_nodo($1); }
         | TK_LIT_FLOAT { $$ = adiciona_nodo($1); }
         | TK_LIT_INT { $$ = adiciona_nodo($1); }
         ; 
%%
int yyerror (char const *s) {
   printf("line %d: %s\n", get_line_number(), s);
   return 1;
}

nodo *adiciona_nodo(valorLexico valor_lexico)
{
    nodo *nodo;

    nodo = malloc(sizeof(nodo)*8);
    nodo->valor_lexico = valor_lexico;
    nodo->filhos = NULL;

    return nodo;
}

nodo *adiciona_nodo_label(char *label)
{
    valorLexico valor_lexico;
    valor_lexico.linha = -1;
    valor_lexico.tipo = OUTRO;
    valor_lexico.tipo_literal = NAO_LITERAL;
    valor_lexico.label = strdup(label);

    nodo *nodo;

    nodo = malloc(sizeof(nodo)*8);
    nodo->valor_lexico = valor_lexico;
    nodo->filhos = NULL;

    return nodo;
}

void adiciona_filho(nodo *pai, nodo *filho) 
{
   if(pai!= NULL && filho!=NULL)
   {
       lseNodo *lse_nodo;
       lse_nodo = malloc(sizeof(lse_nodo)*8);
       lse_nodo->nodo = filho;
       lse_nodo->proximo = NULL;
       lse_nodo->nodo->pai = pai;
       if(pai->filhos == NULL)
       {
           pai->filhos = lse_nodo;
       }
       else
       {
           adiciona_irmao(acha_ultimo_filho(pai->filhos), lse_nodo);
       }
   }
   return;
   
}

//TODO função de alterar nodo
//TODO função de remover nodo
void imprime_arvore(nodo *nodo, int profundidade)
{
    int i = 0;

    if (nodo == NULL)
        return;
    
    for(i = 0; i<profundidade-1; i++) 
    {
        printf("    ");
    }

    if (profundidade == 0)
        printf(nodo->valor_lexico.label);
    else 
    {
        printf("+---");
        printf(nodo->valor_lexico.label);
    }
    printf("\n");

    lseNodo *nodo_f;
    nodo_f = nodo->filhos;
    while(nodo_f!=NULL)
    {
            imprime_arvore(nodo_f->nodo, profundidade+1);
            nodo_f = nodo_f->proximo;
    }
    
    return;
}

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
    novo_irmao->proximo = NULL;
    return;
}

void libera(void *pai)
{
    if(pai == NULL) return;

    nodo *pai_arvore = (nodo*)pai;

    lseNodo *filhos = pai_arvore->filhos;

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

    libera(nodo_irmao);
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
    if(valor_lexico.tipo_literal == STRING)
        free(valor_lexico.valor.valor_string);
    if(valor_lexico.label != NULL)
        free(valor_lexico.label);
    return;
}