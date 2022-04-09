#include "ast.h"
#include "iloc.h"

OperandoCodigo *cria_operando(char* nome, int valor, TipoOperando tipo);
OperandoCodigo *cria_operando_imediato(int valor);
OperandoCodigo *cria_operando_label(char *nome);
OperandoCodigo *cria_operando_registrador(char* nome);
CodigoILOC *_codigo_jump(char* label_destino);

OperandoCodigo *cria_operando_registrador_especial(char* nome);
OperandoCodigo *cria_rfp();
OperandoCodigo *cria_rsp();
OperandoCodigo *cria_rbss();
OperandoCodigo *cria_rpc();
void liga_operandos(OperandoCodigo *primeiro, OperandoCodigo *segundo) ;
void cria_codigo(OperandoCodigo *origem, Operacao operacao, OperandoCodigo *destino);
void codigo_atribuicao(Nodo *nodo);
void codigo_carrega_variavel(Nodo *nodo);
void codigo_carrega_literal(Nodo *nodo);
void codigo_logico(Nodo *nodo);
void codigo_logico_auxiliar(Nodo *nodo, char* labelTrue, char* labelFalse);
void codigo_logico_operacoes(Operacao operacao, char* labelTrue, char* labelFalse);