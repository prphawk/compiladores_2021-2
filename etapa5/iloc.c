#include "iloc.h"

int global_num_rotulos = 1;
int global_num_registradores = 1;

char *gera_nome_rotulo()
{
    return _gera_nome(1);
}

char *gera_nome_registrador()
{
    return _gera_nome(0);
}

char *_gera_nome(int eh_rotulo)
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
    char *numero = calloc(sizeof(char), 10); // +1 for null //TODO sor disse pra checar qnd 10 nao eh o bastante
    snprintf(numero, 11, "%c%d", c, n);
    return numero;
}

OperandoILOC *_cria_operando(char* nome, int valor, TipoOperando tipo) {
    OperandoILOC *operando = malloc(sizeof(OperandoILOC));
    operando->nome = nome;
    operando->valor = valor;
    operando->tipo = tipo;
    operando->proximo = NULL;
    return operando;
}

void libera_codigo(CodigoILOC *codigo) {
    if(codigo == NULL) return;

    libera_codigo(codigo->anterior);

    free(codigo->label);
    codigo->label = NULL;

    libera_operando(codigo->origem);
    libera_operando(codigo->destino);

    free(codigo);
    codigo = NULL;
}

void libera_operando(OperandoILOC *operando) {
    if(operando->proximo == NULL) return;

    libera_operando(operando->proximo);

    free(operando->nome);
    operando->nome = NULL;

    free(operando);
    operando = NULL;
}

char* copy_nome_operando(OperandoILOC *operando) {
    return strdup(operando->nome);
}

void _liga_operandos(OperandoILOC *primeiro, OperandoILOC *segundo) 
{
    primeiro->proximo = segundo;
}

OperandoILOC *lista(OperandoILOC *primeiro, OperandoILOC *segundo) 
{
    _liga_operandos(primeiro, segundo);
    return primeiro;
}

OperandoILOC *cria_operando_remendo_true() {
   return _cria_operando(NULL, 0, REMENDO_TRUE);
}

OperandoILOC *cria_operando_remendo_false() {
   return _cria_operando(NULL, 0, REMENDO_FALSE);
}

OperandoILOC *cria_operando_imediato(int valor) {
   return _cria_operando(NULL, valor, IMEDIATO);
}

OperandoILOC *cria_operando_registrador(char* nome) {
   return _cria_operando(nome, 0, REGISTRADOR);
}

OperandoILOC *cria_operando_label(char* nome) {
   return _cria_operando(nome, 0, LABEL);
}

OperandoILOC *reg_rfp() {
   return _cria_operando(RFP, 0, REGISTRADOR_PONTEIRO);
}

OperandoILOC *reg_rsp() {
   return _cria_operando(RSP, 0, REGISTRADOR_PONTEIRO);
}

OperandoILOC *reg_rbss() {
   return _cria_operando(RBSS, 0, REGISTRADOR_PONTEIRO);
}

OperandoILOC *reg_rpc() {
   return _cria_operando(RPC, 0, REGISTRADOR_PONTEIRO);
}