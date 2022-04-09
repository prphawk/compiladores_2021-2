#include "ast.h"
#include "iloc.h"

void codigo_literal(Nodo *nodo);
OperandoCodigo *cria_operando(char* nome, int valor, TipoOperando tipo);
OperandoCodigo *cria_operando_imediato(int valor);
OperandoCodigo *cria_operando_label(char *nome, int valor);
OperandoCodigo *cria_operando_registrador(char* nome);
OperandoCodigo *cria_operando_registrador_especial(char* nome);
OperandoCodigo *cria_rfp();
OperandoCodigo *cria_rsp();
OperandoCodigo *cria_rbss();
OperandoCodigo *cria_rpc();
void liga_operandos(OperandoCodigo *primeiro, OperandoCodigo *segundo) ;
void cria_codigo(OperandoCodigo *origem, Operacao operacao, OperandoCodigo *destino);
void codigo_literal(Nodo *nodo);
void codigo_atribuicao(Nodo *nodo, int deslocamento, int eh_escopo_global);
void codigo_logico(Nodo *nodo);
void codigo_logico_auxiliar(Nodo *nodo, char* labelTrue, char* labelFalse);
void codigo_logico_operacoes(Operacao operacao, char* labelTrue, char* labelFalse);