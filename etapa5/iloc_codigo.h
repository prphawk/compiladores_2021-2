#include "ast.h"
#include "iloc.h"

CodigoILOC *instrucao_jump(char* label_destino);
CodigoILOC *_cria_codigo(OperandoCodigo *origem, Operacao operacao, OperandoCodigo *destino);
CodigoILOC *instrucao_loadI(int valor);

void _append_codigo(Nodo* nodo, CodigoILOC *codigo);
void cria_codigo_e_append(Nodo* nodo, OperandoCodigo *origem, Operacao operacao, OperandoCodigo *destino);
void codigo_atribuicao(Nodo *variavel, Nodo *atribuicao, Nodo *expressao);
void codigo_carrega_variavel(Nodo *nodo);
void codigo_carrega_literal(Nodo *nodo);
void codigo_logico(Nodo *nodo);
void codigo_logico_auxiliar(char *label, Nodo *nodo, char* labelTrue, char* labelFalse);
void codigo_logico_operacoes(char *label, Operacao operacao, char* labelTrue, char* labelFalse);
void codigo_expr_unaria(Nodo *operacao, Nodo *nodo);
void codigo_sub(Nodo *operacao, Nodo *nodo);