#include "ast.h"
#include "iloc.h"
#include "tabela_simbolos.h"

void _append(Nodo *nodo, CodigoILOC *codigo);
CodigoILOC *_append_codigo(CodigoILOC *lst, CodigoILOC *new_lst);
void codigo_append_nodo(Nodo *pai, Nodo *filho);
void _cria_codigo_append(Nodo *nodo, OperandoILOC *origem, OperacaoILOC operacao, OperandoILOC *destino);
int tem_buracos(Nodo *nodo);

void codigo_atribuicao(Nodo *variavel, Nodo *atribuicao, Nodo *expressao);
void codigo_update_deslocamento(Nodo *nodo);
void codigo_carrega_variavel(Nodo *nodo);
void codigo_expr_logica_booleano(Nodo *nodo, int valor);
void codigo_carrega_literal(Nodo *nodo);
void codigo_logico(Nodo *nodo);
void codigo_logico_auxiliar(char *label, Nodo *nodo, char* labelTrue, char* labelFalse);
void codigo_logico_operacoes(Nodo *nodo, char *label, OperacaoILOC operacao, char* labelTrue, char* labelFalse);
void codigo_expr_unaria(Nodo *operador, Nodo *expr);
void codigo_sub(Nodo *operador, Nodo *expr);
void codigo_not(Nodo *operador, Nodo *expr);

void codigo_if_else(Nodo *nodo, Nodo *expressao, Nodo *bloco_true, Nodo *bloco_false);
void codigo_avalia_expr(Nodo *operador, Nodo *expr, char* reg_resultado_nome, char* operacao_label);
int operacao_iloc_binaria_nodo(Nodo *nodo_operador);
void codigo_expr_aritmetica(Nodo *esq, Nodo *operador, Nodo *dir);

void codigo_expr_logica(Nodo *esq, Nodo *nodo_operador, Nodo *dir);
void codigo_expr_logica_relacional(Nodo *esq, Nodo *operador, Nodo *dir);
void codigo_expr_logica_and(Nodo *esq, Nodo *operador, Nodo *dir);
void codigo_expr_logica_or(Nodo *esq, Nodo *operador, Nodo *dir);

CodigoILOC *atribui_booleano(Nodo *expressao, char* rotulo_final);

void converte_para_logica(Nodo *expressao);
void codigo_while(Nodo *nodo, Nodo *expressao, Nodo *bloco);
void codigo_for(Nodo *nodo, Nodo *atribuicao_inicial, Nodo *expressao, Nodo *atribuicao_final, Nodo *bloco);

CodigoILOC *codigo_compara_logico(OperandoILOC *r1, OperandoILOC *op_label_true, OperandoILOC *op_label_false);
CodigoILOC* intrucoes_cmp_NE_0(OperandoILOC *r1);
CodigoILOC *instrucao_nop(char* label);
CodigoILOC* instrucao_cbr(OperandoILOC *r1, OperandoILOC *op_label_true, OperandoILOC *op_label_false);
CodigoILOC *instrucao_loadI(int valor, char *label);
CodigoILOC *instrucao_loadI_reg(int valor, char *label, OperandoILOC *r2);
CodigoILOC *instrucao_jumpI(OperandoILOC *destino);
