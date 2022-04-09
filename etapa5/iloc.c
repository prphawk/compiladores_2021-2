#include "iloc.h"

int global_num_rotulos = 1;
int global_num_registradores = 1;

char *gera_nome_rotulo()
{
    return gera_nome(1);
}

char *gera_nome_registrador()
{
    return gera_nome(0);
}

char *gera_nome(int eh_rotulo)
{
    int n;
    char c;
    if (eh_rotulo == 0)
    {
        n = global_num_rotulos;
        global_num_rotulos++;
        c = 'L';
    }
    else
    {
        n = global_num_registradores;
        global_num_registradores++;
        c = 'r';
    }
    char *numero = calloc(sizeof(char), 10); // +1 for null
    snprintf(numero, 11, "%c%d", c, n);
    return numero;
}

OperandoCodigo *cria_operando(char* nome, int valor, TipoOperando tipo) {
    OperandoCodigo *operando = malloc(sizeof(OperandoCodigo));
    operando->nome = nome;
    operando->valor = valor;
    operando->tipo = tipo;
    operando->proximo = NULL;
    return operando;
}

void liga_operandos(OperandoCodigo *primeiro, OperandoCodigo *segundo) 
{
    primeiro->proximo = segundo;
}

//TODO n é usada
void desliga_operando(OperandoCodigo *primeiro)
{
    primeiro->proximo = NULL;
}

OperandoCodigo *cria_operando_imediato(int valor) {
   return cria_operando(NULL, valor, IMEDIATO);
}

OperandoCodigo *cria_operando_registrador(char* nome) {
   return cria_operando(nome, 0, REGISTRADOR);
}

OperandoCodigo *cria_operando_label(char* nome) {
   return cria_operando(nome, 0, LABEL);
}

OperandoCodigo *cria_rfp() {
   return cria_operando(RFP, 0, REGISTRADOR_ESPECIAL);
}

OperandoCodigo *cria_rsp() {
   return cria_operando(RSP, 0, REGISTRADOR_ESPECIAL);
}

OperandoCodigo *cria_rbss() {
   return cria_operando(RBSS, 0, REGISTRADOR_ESPECIAL);
}

OperandoCodigo *cria_rpc() {
   return cria_operando(RPC, 0, REGISTRADOR_ESPECIAL);
}