#include "ast.h"
#include "iloc.h"

void codigo_literal(Nodo *nodo);
OperandoCodigo *cria_operando(char* nome, int valor, TipoOperando tipo);
OperandoCodigo *cria_operando_imediato(int valor);
OperandoCodigo *cria_operando_registrador(char* nome);
void cria_codigo(OperandoCodigo *origem, Operacao operacao, OperandoCodigo *destino);