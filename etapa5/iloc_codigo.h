#include "ast.h"
#include "iloc.h"
#include "tabela_simbolos.h"

CodigoILOC *instrucao_loadI(int valor);
CodigoILOC *instrucao_jumpI(OperandoILOC* destino);

void _append(Nodo *nodo, CodigoILOC *codigo);
void _cria_codigo_append(Nodo *nodo, OperandoILOC *origem, OperacaoILOC operacao, OperandoILOC *destino);
void codigo_atribuicao(Nodo *variavel, Nodo *atribuicao, Nodo *expressao);
void codigo_carrega_variavel(Nodo *nodo);
void codigo_expr_logica_booleano(Nodo *nodo, int valor);
void codigo_carrega_literal(Nodo *nodo);
void codigo_logico(Nodo *nodo);
void codigo_logico_auxiliar(char *label, Nodo *nodo, char* labelTrue, char* labelFalse);
void codigo_logico_operacoes(Nodo *nodo, char *label, OperacaoILOC operacao, char* labelTrue, char* labelFalse);
void codigo_expr_unaria(Nodo *operador, Nodo *expr);
void codigo_sub(Nodo *operador, Nodo *expr);
void codigo_not(Nodo *operador, Nodo *expr);

void codigo_avalia_expr(Nodo *operador, Nodo *expr, char* reg_resultado_nome, char* operacao_label);
int operacao_iloc_binaria_nodo(Nodo *nodo_operador);
void codigo_expr_aritmetica(Nodo *esq, Nodo *operador, Nodo *dir);

CodigoILOC* intrucoes_cmp_NE_0(OperandoILOC *r1);
CodigoILOC *instrucao_nop(char* label);
CodigoILOC* instrucao_cbr(OperandoILOC *r1, OperandoILOC *op_label_true, OperandoILOC *op_label_false);
CodigoILOC *codigo_compara_logico(OperandoILOC *r1, OperandoILOC *op_label_true, OperandoILOC *op_label_false);

Remendo *cria_remendo(OperandoILOC *remendo_operando);
void append_remendo(Remendo *remendo_lst, OperandoILOC *remendo_operando);
void remenda(Remendo *buracos, OperandoILOC *argamassa);
Remendo *concat_remendos(Remendo *lst1, Remendo *lst2);
