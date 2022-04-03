%{
#include<stdio.h>
#include<string.h>
#include "main.h"
#include "ast.h"
#include "tabela_simbolos.h"
int yylex(void);
int yyerror (char const *s);
extern int get_line_number (void);
extern void *arvore;
%}
%define parse.error verbose
%code requires {
    #include "valor_lexico.h"
    #include "ast.h"
    #include "tabela_simbolos.h"
}
%union {
   ValorLexico valor_lexico;
   struct Nodo *nodo;
   TipoSimbolo tipo_simbolo;
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
%token<valor_lexico> '+' '-' '|' '*' '/' '<' '>' '=' '!' '&' '%' '#' '^' '$' '?' 
%token TOKEN_ERRO

%type<nodo> literal
%type<nodo> cabeca_lista_nome_variavel_local
%type<nodo> variavel_ou_literal
%type<nodo> variavel_ou_vetor
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
%type<nodo> operador_binario_logico
%type<nodo> operando_logico
%type<nodo> expr_ternaria
%type<nodo> expr_bin_aritmetica
%type<nodo> expr_bin_aritmetica_1
%type<nodo> expr_bin_aritmetica_2
%type<nodo> expr_bin_aritmetica_3
%type<nodo> expr_bin_aritmetica_4
%type<nodo> expr_unaria_aritmetica
%type<nodo> expr_parenteses_aritmetica
%type<nodo> expr_bin_logica
%type<nodo> expr_parenteses_logica
%type<nodo> operando_aritmetico
%type<nodo> chamada_funcao
%type<nodo> lista_comandos
%type<nodo> corpo
%type<nodo> lista_argumentos
%type<nodo> argumentos
%type<nodo> cabecalho
%type<nodo> cabecalho_1
%type<nodo> cabecalho_2
%type<nodo> declaracao_funcao
%type<nodo> declaracao
%type<nodo> declaracoes
%type<nodo> programa

%type<tipo_simbolo> tipo //TODO ver se n da merda na ast...
%type<nodo> variavel
%type<nodo> vetor
%%

programa: declaracoes { $$ = $1; arvore = $$; };

declaracoes: declaracao declaracoes 
            {
                if ($1!=NULL) { adiciona_filho($1, $2); $$ = $1; }
                else $$ = $2;       
            } 
            | { $$ = NULL;}
            ;

declaracao: declaracao_variavel_global { $$ = NULL; } | declaracao_funcao { $$ = $1; };

declaracao_variavel_global: TK_PR_STATIC tipo lista_nome_variavel_global ';'    { insere_tipo_identificador_pilha($2); }
                        | tipo lista_nome_variavel_global ';'                   { insere_tipo_identificador_pilha($1); }
                        ;

lista_nome_variavel_global: nome_variavel_global | nome_variavel_global ',' lista_nome_variavel_global;

// TODO checar se pode declarar vetores de string. parece que não
nome_variavel_global: TK_IDENTIFICADOR                      
                    { 
                        insere_variavel_sem_tipo_pilha($1); 
                        libera_vlex($1); 
                    }
                    | TK_IDENTIFICADOR '[' TK_LIT_INT ']'   //TODO mudar indexador para expr aritmetica quando souber como pegar o valor
                    { 
                        insere_vetor_sem_tipo_pilha($1, $3.valor_int); 
                        libera_vlex($1); libera_vlex($3); 
                    };

corpo: '{' lista_comandos corpo_1 { $$ = $2; }
corpo_1: '}' { desempilha(); libera_ultima_funcao(); }

declaracao_funcao: cabecalho corpo //TODO como botar os parametros do cabeçalho no escopo do corpo?? 
                {
                    adiciona_filho($1, $2);
                    $$ = $1;
                };

cabecalho: TK_PR_STATIC cabecalho_1 { $$ = $2; } | cabecalho_1 { $$ = $1; };
cabecalho_1: cabecalho_2 cabecalho_3 parametros ')' 
            { 
                adiciona_argumentos_escopo_anterior($1); 
                $$ = $1; 
            }
cabecalho_2: tipo TK_IDENTIFICADOR
            { 
                printf(">> cheguei aqui\n");
                insere_funcao_pilha($1, $2);
                Nodo *novo_nodo = adiciona_nodo($2); 
                novo_nodo->tipo =  $1;
                $$ = novo_nodo;
            };
cabecalho_3:  '(' { empilha(); };



parametros: lista_parametros | ;

lista_parametros: parametro | parametro ',' lista_parametros;

parametro: tipo TK_IDENTIFICADOR            { insere_argumento_sem_funcao($1, $2); libera_vlex($2); } 
        | TK_PR_CONST tipo TK_IDENTIFICADOR { insere_argumento_sem_funcao($2, $3); libera_vlex($3); };

//TODO checar se eles aparecem na arvore (nao deveriam)

tipo: TK_PR_INT     { $$ = TIPO_INT;    }
    | TK_PR_FLOAT   { $$ = TIPO_FLOAT;  }
    | TK_PR_CHAR    { $$ = TIPO_CHAR;   }
    | TK_PR_BOOL    { $$ = TIPO_BOOL;   }
    | TK_PR_STRING  { $$ = TIPO_STRING; }
    ;

lista_comandos: comando_simples ';' lista_comandos 
    { 
        if($1==NULL)
            $$ = $3;
        else {
            adiciona_filho($1, $3);
            $$ = $1;
        }
    } //caso $1 seja apenas um identificador, volta nulo e n podemos processar o $3
    //TODO checar correção.
    | { $$ = NULL; };

bloco_comandos: bloco_comandos_inicio_escopo lista_comandos bloco_comandos_fim_escopo { $$ = $2; } ;
bloco_comandos_inicio_escopo: '{'  { empilha();    } ;
bloco_comandos_fim_escopo: '}'     { desempilha(); } ;

chamada_funcao: TK_IDENTIFICADOR'('lista_argumentos')' { 
            Nodo *novo_nodo = adiciona_nodo_label_concat("call ", $1.label);
            //adiciona_filho(novo_nodo, adiciona_nodo($1));
            adiciona_filho(novo_nodo, $3);
            verifica_funcao_no_escopo($1, $3, novo_nodo);
            $$ = novo_nodo;
            libera_vlex($1); //precisa liberar pq o identificador é substituido na arvore!!
        };

comando_simples: declaracao_var_local   { $$ = $1;}
               | comando_atribuicao     { $$ = $1;}
               | comando_entrada        { $$ = $1;}
               | comando_saida          { $$ = $1;}
               | comando_shift          { $$ = $1;}
               | comando_retorno        { $$ = $1;}
               | TK_PR_BREAK            { $$ = adiciona_nodo_label("break");    }
               | TK_PR_CONTINUE         { $$ = adiciona_nodo_label("continue"); }
               | comando_condicional    { $$ = $1;}
               | comando_iterativo      { $$ = $1;}
               | chamada_funcao         { $$ = $1;}
               | bloco_comandos         { $$ = $1;}
               ;

declaracao_var_local: TK_PR_STATIC TK_PR_CONST tipo lista_nome_variavel_local   { $$ = $4; insere_tipo_identificador_pilha($3); inicializacao_nodo($3, $4); }
                     | TK_PR_CONST tipo lista_nome_variavel_local               { $$ = $3; insere_tipo_identificador_pilha($2); inicializacao_nodo($2, $3); }
                     | TK_PR_STATIC tipo lista_nome_variavel_local              { $$ = $3; insere_tipo_identificador_pilha($2); inicializacao_nodo($2, $3); }
                     | tipo lista_nome_variavel_local                           { $$ = $2; insere_tipo_identificador_pilha($1); inicializacao_nodo($1, $2); }
                     ;

lista_nome_variavel_local: cabeca_lista_nome_variavel_local ',' lista_nome_variavel_local
                        {
                            if ($1!=NULL) {
                                adiciona_filho($1, $3);
                                $$ = $1;
                            }
                            else $$ = $3;       
                        }   
                        | cabeca_lista_nome_variavel_local { $$ = $1;}
                        ;

cabeca_lista_nome_variavel_local: TK_IDENTIFICADOR TK_OC_LE variavel_ou_literal {
                                    Nodo *novo_nodo = adiciona_nodo($2);
                                    adiciona_filho(novo_nodo, adiciona_nodo($1));
                                    adiciona_filho(novo_nodo, $3);
                                    $$ = novo_nodo;
                                    insere_variavel_sem_tipo_pilha($1);
                                }
                                | TK_IDENTIFICADOR { 
                                    insere_variavel_sem_tipo_pilha($1);
                                    libera_vlex($1), $$ = NULL;
                                }
                                ;

variavel_ou_literal: variavel { $$ = $1; } | literal { $$ = $1; };

variavel_ou_vetor: variavel { $$ = $1; } | vetor { $$ = $1; };

comando_atribuicao: variavel_ou_vetor '=' expressao
                    {
                        Nodo *novo_nodo = adiciona_nodo($2);
                        adiciona_filho(novo_nodo, $1);
                        adiciona_filho(novo_nodo, $3);
                        $$ = novo_nodo;
                        verifica_atribuicao($1, novo_nodo, $3);
                    };

// TODO checar tipo DE TODOS ESSES
comando_entrada: TK_PR_INPUT variavel
                {
                    Nodo *novo_nodo = adiciona_nodo_label("input");
                    adiciona_filho(novo_nodo, $2);
                    $$ = novo_nodo;
                    verifica_input(novo_nodo, $2);

                };

comando_saida: TK_PR_OUTPUT variavel_ou_literal 
                {
                    Nodo *novo_nodo = adiciona_nodo_label("output");
                    adiciona_filho(novo_nodo, $2);
                    $$ = novo_nodo;
                    verifica_output(novo_nodo, $2);
                };

comando_shift: variavel_ou_vetor TK_OC_SL TK_LIT_INT 
                {
                    Nodo *novo_nodo = adiciona_nodo($2);
                    adiciona_filho(novo_nodo, $1);
                    adiciona_filho(novo_nodo, adiciona_nodo($3));
                    $$ = novo_nodo;
                    verifica_shift($3);
                    insere_literal_pilha(TIPO_INT,$3);
                }
               | variavel_ou_vetor TK_OC_SR TK_LIT_INT 
               {
                    Nodo *novo_nodo = adiciona_nodo($2);
                    adiciona_filho(novo_nodo, $1);
                    adiciona_filho(novo_nodo, adiciona_nodo($3));
                    $$ = novo_nodo;
                    verifica_shift($3);
                    insere_literal_pilha(TIPO_INT,$3);
                }
               ;

comando_retorno: TK_PR_RETURN expressao
                {
                    Nodo *novo_nodo = adiciona_nodo_label("return");
                    adiciona_filho(novo_nodo, $2);
                    $$ = novo_nodo;
                    verifica_return(novo_nodo, $2);
                };

comando_condicional: TK_PR_IF '(' expressao ')' bloco_comandos 
                        {
                            Nodo *novo_nodo = adiciona_nodo_label("if");
                            adiciona_filho(novo_nodo, $3);
                            adiciona_filho(novo_nodo, $5);
                            $$ = novo_nodo;
                        }
                     | TK_PR_IF '(' expressao ')' bloco_comandos TK_PR_ELSE bloco_comandos
                        {
                            Nodo *novo_nodo = adiciona_nodo_label("if");
                            adiciona_filho(novo_nodo, $3);
                            adiciona_filho(novo_nodo, $5);
                            adiciona_filho(novo_nodo, $7);
                            $$ = novo_nodo;
                        }
                     ;

comando_iterativo: TK_PR_FOR '(' comando_atribuicao ':' expressao ':' comando_atribuicao')' bloco_comandos
                    {
                        Nodo *novo_nodo = adiciona_nodo_label("for");
                        adiciona_filho(novo_nodo, $3);
                        adiciona_filho(novo_nodo, $5);
                        adiciona_filho(novo_nodo, $7);
                        adiciona_filho(novo_nodo, $9);
                        $$ = novo_nodo;
                    }
                  | TK_PR_WHILE '('expressao')' TK_PR_DO bloco_comandos
                  {
                      Nodo *novo_nodo = adiciona_nodo_label("while");
                      adiciona_filho(novo_nodo, $3);
                      adiciona_filho(novo_nodo, $6);
                      $$ = novo_nodo;
                  }
                  ;

argumentos: expressao',' argumentos 
            {;
                adiciona_filho($1, $3);
                $$ = $1;
            }
            | expressao { $$ = $1; };

lista_argumentos: argumentos { $$ = $1; } | { $$ = NULL; };

literal: TK_LIT_CHAR        { $$ = adiciona_nodo($1); insere_literal_pilha(TIPO_CHAR,   $1);}
         | TK_LIT_STRING    { $$ = adiciona_nodo($1); insere_literal_pilha(TIPO_STRING, $1);}
         | TK_LIT_TRUE      { $$ = adiciona_nodo($1); insere_literal_pilha(TIPO_BOOL,   $1);}
         | TK_LIT_FALSE     { $$ = adiciona_nodo($1); insere_literal_pilha(TIPO_BOOL,   $1);}
         | TK_LIT_FLOAT     { $$ = adiciona_nodo($1); insere_literal_pilha(TIPO_FLOAT,  $1);}
         | TK_LIT_INT       { $$ = adiciona_nodo($1); insere_literal_pilha(TIPO_INT,    $1);}
         ;

operador_binario_prec1: '^'     { $$ = adiciona_nodo($1); };
operador_binario_prec2: '*'     { $$ = adiciona_nodo($1); } 
                        | '/'   { $$ = adiciona_nodo($1); } 
                        | '%'   { $$ = adiciona_nodo($1); }
                        ;
operador_binario_prec3: '+'     { $$ = adiciona_nodo($1); } 
                        | '-'   { $$ = adiciona_nodo($1); }
                        ;
operador_binario_prec4: '&'     { $$ = adiciona_nodo($1); } 
                        | '|'   { $$ = adiciona_nodo($1); }
                        ;
operador_binario_prec5: '<'         { $$ = adiciona_nodo($1); } 
                        | '>'       { $$ = adiciona_nodo($1); } 
                        | TK_OC_LE  { $$ = adiciona_nodo($1); }
                        | TK_OC_EQ  { $$ = adiciona_nodo($1); }
                        | TK_OC_GE  { $$ = adiciona_nodo($1); }
                        | TK_OC_NE  { $$ = adiciona_nodo($1); }
                        | operador_binario_logico { $$ = $1; }
                        ;

operador_asterisco: '*' { $$ = adiciona_nodo($1); } 

operador_unario: '-' { $$ = adiciona_nodo($1); }
               | '+' { $$ = adiciona_nodo($1); } 
               | '!' { $$ = adiciona_nodo($1); } 
               | '&' { $$ = adiciona_nodo($1); } 
               | '?' { $$ = adiciona_nodo($1); }
               | '#' { $$ = adiciona_nodo($1); }
               ;

operador_binario_logico: TK_OC_OR { $$ = adiciona_nodo($1); } | TK_OC_AND { $$ = adiciona_nodo($1); };

expressao: expr_ternaria        { $$ = $1; } //fazer escadinha? pra evitar a repetição ali do ternario. v
        | expr_bin_aritmetica   { $$ = $1; }
        | expr_bin_logica       { $$ = $1; }
        ;

expr_ternaria: expr_bin_aritmetica '?' expressao ':' expressao 
            { 
                libera_vlex($2); //não usaremos, mas foi alocado!!
                Nodo *novo_nodo = adiciona_nodo_label("?:"); 
                adiciona_filho(novo_nodo, $1); 
                adiciona_filho(novo_nodo, $3); 
                adiciona_filho(novo_nodo, $5); 
                $$ = novo_nodo; 
                verifica_expr_ternaria($1, $3, $5, novo_nodo);
            }
            | expr_bin_logica '?' expressao ':' expressao
            {
                libera_vlex($2);
                Nodo *novo_nodo = adiciona_nodo_label("?:");
                adiciona_filho(novo_nodo, $1);
                adiciona_filho(novo_nodo, $3);
                adiciona_filho(novo_nodo, $5);
                $$ = novo_nodo;
                verifica_expr_ternaria($1, $3, $5, novo_nodo);
            };

expr_bin_aritmetica: expr_bin_aritmetica_1 { $$ = $1; }
                | expr_bin_aritmetica operador_binario_prec5 expr_bin_aritmetica_1
                {
                    adiciona_filho($2, $1);
                    adiciona_filho($2, $3);
                    $$ = $2;
                    verifica_expr_binaria($1, $2, $3);
                };
expr_bin_aritmetica_1: expr_bin_aritmetica_2 { $$ = $1; }
                | expr_bin_aritmetica_1 operador_binario_prec4 expr_bin_aritmetica_2
                {
                    adiciona_filho($2, $1);
                    adiciona_filho($2, $3);
                    $$ = $2;
                    verifica_expr_binaria($1, $2, $3);
                }
			    ;
expr_bin_aritmetica_2: expr_bin_aritmetica_3 { $$ = $1; }
                | expr_bin_aritmetica_2 operador_binario_prec3 expr_bin_aritmetica_3
                {
                    adiciona_filho($2, $1);
                    adiciona_filho($2, $3);
                    $$ = $2;
                    verifica_expr_binaria($1, $2, $3);
                }
				;
expr_bin_aritmetica_3: expr_bin_aritmetica_4 { $$ = $1; }
					| expr_bin_aritmetica_3 operador_binario_prec2 expr_bin_aritmetica_4
					{
						adiciona_filho($2, $1);
						adiciona_filho($2, $3);
						$$ = $2;
                        verifica_expr_binaria($1, $2, $3);

					}
					;
expr_bin_aritmetica_4: expr_unaria_aritmetica { $$ = $1; }
					| expr_bin_aritmetica_4 operador_binario_prec1 expr_unaria_aritmetica
					{
						adiciona_filho($2, $1);
						adiciona_filho($2, $3);
						$$ = $2;
                        verifica_expr_binaria($1, $2, $3);
					}
					;            
expr_unaria_aritmetica: 
                expr_parenteses_aritmetica 
                { 
                    $$ = $1; 
                    //verifica_expr_unaria(NULL, $1);
                }
				| operador_unario expr_parenteses_aritmetica 
				{
					adiciona_filho($1, $2);
					$$ = $1;
                    verifica_expr_unaria($1, $2);
				}
				| operador_asterisco expr_unaria_aritmetica
				{
					adiciona_filho($1, $2);
					$$ = $1;
                    verifica_expr_unaria($1, $2);
				}
				;

expr_parenteses_aritmetica: operando_aritmetico         { $$ = $1; } 
                        | '(' expr_bin_aritmetica ')'   { $$ = $2; }
                        ; 

operando_aritmetico: variavel           { $$ = $1; }
                    | vetor             { $$ = $1; }
                    | chamada_funcao    { $$ = $1; }
                    | TK_LIT_FLOAT      { $$ = adiciona_nodo($1); insere_literal_pilha(TIPO_FLOAT, $1); }
                    | TK_LIT_INT        { $$ = adiciona_nodo($1); insere_literal_pilha(TIPO_INT, $1); }
                    ;

variavel: TK_IDENTIFICADOR 
        { 
            Nodo *novo_nodo = adiciona_nodo($1); 
            verifica_variavel_no_escopo(novo_nodo); 
            $$ = novo_nodo;
        };

vetor: TK_IDENTIFICADOR'['expr_bin_aritmetica']'
                { 
                    Nodo *novo_nodo = adiciona_nodo_label("[]");
                    adiciona_filho(novo_nodo, adiciona_nodo($1));
                    adiciona_filho(novo_nodo, $3);
                    verifica_vetor_no_escopo(novo_nodo);
                    $$ = novo_nodo;
                };

operando_logico: TK_LIT_TRUE    { $$ = adiciona_nodo($1); insere_literal_pilha(TIPO_BOOL, $1); } 
                | TK_LIT_FALSE  { $$ = adiciona_nodo($1); insere_literal_pilha(TIPO_BOOL, $1); };
            //| expr_bin_aritmetica { $$ = $1; }; //TODO E3

expr_bin_logica: expr_bin_logica operador_binario_logico expr_parenteses_logica 
                {
                    adiciona_filho($2, $1);
                    adiciona_filho($2, $3);
                    $$ = $2;
                    verifica_expr_binaria($1, $2, $3);
                }
                | expr_parenteses_logica operador_binario_logico expr_parenteses_logica 
                {
                    adiciona_filho($2, $1);
                    adiciona_filho($2, $3);
                    $$ = $2;
                    verifica_expr_binaria($1, $2, $3);
                };

expr_parenteses_logica: operando_logico { $$ = $1; } | '(' expr_bin_logica ')' { $$ = $2; };

%%
int yyerror (char const *s) {
   printf("line %d: %s\n", get_line_number(), s);
   return 1;
}