#include "iloc_codigo.h"

CodigoILOC *global_codigo = NULL;

void cria_codigo(OperandoCodigo *origem, Operacao operacao, OperandoCodigo *destino)
{
    CodigoILOC *codigo = malloc(sizeof(CodigoILOC));
    codigo->origem = origem;
    codigo->operacao = operacao;
    codigo->destino = destino;

    codigo->anterior = global_codigo;
    global_codigo = codigo;
}

OperandoCodigo *cria_operando(char* nome, int valor, TipoOperando tipo) {
    OperandoCodigo *operando = malloc(sizeof(OperandoCodigo));
    operando->nome = nome;
    operando->valor = valor;
    operando->tipo = tipo;
    operando->proximo = NULL;
    return operando;
}

OperandoCodigo *cria_operando_imediato(int valor) {
   return cria_operando(NULL, valor, IMEDIATO);
}

OperandoCodigo *cria_operando_registrador(char* nome) {
   return cria_operando(nome, 0, REGISTRADOR);
}

// loadI c1 => r2 // r2 = c1
void codigo_literal(Nodo *nodo) {

    int valor = nodo->valor_lexico.valor_int; //2.3: Simplificações para a Geração de Código

    char* registrador = gera_nome_registrador();

    OperandoCodigo *origem = cria_operando_imediato(valor);

    OperandoCodigo *destino = cria_operando_registrador(registrador);

    cria_codigo(origem, LOADI, destino);
    
    nodo->codigo = global_codigo;
    nodo->resultado = destino;
}