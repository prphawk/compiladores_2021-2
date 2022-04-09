#include "ast.h"
#include "iloc.h"

void nodo_loadI(Nodo *nodo);
OperandoCodigo *cria_operando(char* nome, int valor, TipoOperando tipo);
OperandoCodigo *cria_operando_imediato(int valor);
OperandoCodigo *cria_operando_registrador(char* nome);
CodigoILOC * cria_codigo(OperandoCodigo *origem, Operacao operacao, OperandoCodigo *destino);
CodigoILOC *codigo_jumpI(char* label_destino);

void nodo_loadI(Nodo *nodo);