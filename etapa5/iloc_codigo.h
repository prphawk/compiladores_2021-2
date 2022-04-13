#include "ast.h"
#include "iloc.h"

CodigoILOC *_cria_codigo(OperandoILOC *origem, OperacaoILOC operacao, OperandoILOC *destino);
CodigoILOC *instrucao_loadI(int valor);
CodigoILOC *instrucao_jump(OperandoILOC* destino);

void _append_codigo(Nodo *nodo, CodigoILOC *codigo);
void cria_codigo_e_append(Nodo *nodo, OperandoILOC *origem, OperacaoILOC operacao, OperandoILOC *destino);
void imprime_codigo_final();
void imprime_codigo(CodigoILOC *codigo);
void imprime_operando(OperandoILOC *operando);
void codigo_atribuicao(Nodo *variavel, Nodo *atribuicao, Nodo *expressao);
void codigo_carrega_variavel(Nodo *nodo);
void codigo_carrega_literal(Nodo *nodo, int valor);
void codigo_logico(Nodo *nodo);
void codigo_logico_auxiliar(char *label, Nodo *nodo, char* labelTrue, char* labelFalse);
void codigo_logico_operacoes(Nodo *nodo, char *label, OperacaoILOC operacao, char* labelTrue, char* labelFalse);
void codigo_expr_unaria(Nodo *operador, Nodo *expr);
void codigo_sub(Nodo *operador, Nodo *expr);
void codigo_not(Nodo *operador, Nodo *expr);