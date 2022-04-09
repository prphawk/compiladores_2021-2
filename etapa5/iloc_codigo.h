#include "ast.h"
#include "iloc.h"

CodigoILOC *_codigo_jump(char* label_destino);
void cria_codigo(OperandoCodigo *origem, Operacao operacao, OperandoCodigo *destino);
void codigo_atribuicao(Nodo *nodo);
void codigo_carrega_variavel(Nodo *nodo);
void codigo_carrega_literal(Nodo *nodo);
void codigo_logico(Nodo *nodo);
void codigo_logico_auxiliar(Nodo *nodo, char* labelTrue, char* labelFalse);
void codigo_logico_operacoes(Operacao operacao, char* labelTrue, char* labelFalse);