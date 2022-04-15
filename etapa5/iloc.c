#include "iloc.h"

int global_num_rotulos = 1;
int global_num_registradores = 1;

extern int print_ILOC_intermed_global;

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
    if (eh_rotulo)
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

OperandoILOC *copia_operando(OperandoILOC *operando) {

   if(operando == NULL) return NULL;

    OperandoILOC *copia = malloc(sizeof(OperandoILOC));
    copia->nome = copia_nome(operando->nome);
    copia->valor = operando->valor;
    copia->tipo = operando->tipo;
    copia->proximo = copia_operando(operando->proximo);
    return copia;
}

void libera_codigo(CodigoILOC *codigo) {
    if(codigo == NULL) return;

    libera_codigo(codigo->anterior);

    libera_nome(codigo->label);
    codigo->label = NULL;

    libera_operando(codigo->origem);
    libera_operando(codigo->destino);

    free(codigo);
    codigo = NULL;
}

void libera_nome(char *nome) {
   if(nome != NULL) free(nome);
}

void libera_operando(OperandoILOC *operando) {
    if(operando == NULL) return;

    libera_operando(operando->proximo);


   if(operando->tipo != REGISTRADOR_PONTEIRO) {
     libera_nome(operando->nome);
      operando->nome = NULL;
   }

   free(operando); 
   operando = NULL;
}

void libera_remendo(Remendo *remendo) {
    if(remendo == NULL) return;

    libera_remendo(remendo->proximo);

   free(remendo);
   remendo = NULL;
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

OperandoILOC *gera_operando_remendo() {
   return _cria_operando(NULL, 0, REMENDO);
}

OperandoILOC *gera_operando_imediato(int valor) {
   return _cria_operando(NULL, valor, IMEDIATO);
}

OperandoILOC *gera_operando_registrador(char* nome) {
   return _cria_operando(nome, 0, REGISTRADOR);
}

OperandoILOC *gera_operando_rotulo(char* nome) {
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

char* copia_nome(char *nome) {
   if(nome == NULL) return NULL;
   return strdup(nome);
}

CodigoILOC *copia_codigo(CodigoILOC *codigo) {

	if(codigo == NULL) return NULL;

	CodigoILOC *copia = malloc(sizeof(CodigoILOC));
	copia->label = copia_nome(codigo->label);
	copia->origem = copia_operando(codigo->origem);
	copia->operacao = codigo->operacao;
	copia->destino = copia_operando(codigo->destino);
	copia->anterior = copia_codigo(codigo->anterior);

   return copia;
}

CodigoILOC *_cria_codigo(OperandoILOC *origem, OperacaoILOC operacao, OperandoILOC *destino)
{
    CodigoILOC *codigo = malloc(sizeof(CodigoILOC));
    codigo->label = NULL;
    codigo->origem = origem;
    codigo->operacao = operacao;
    codigo->destino = destino;
    codigo->anterior = NULL;

    return codigo;
}

CodigoILOC *_cria_codigo_com_label(char *label, OperandoILOC *origem, OperacaoILOC operacao, OperandoILOC *destino)
{
   CodigoILOC *codigo = _cria_codigo(origem, operacao, destino);
   codigo->label = label;
   return codigo;
}

void imprime_codigo(CodigoILOC *codigo)
{
   if(codigo!=NULL)
   {
      imprime_codigo(codigo->anterior);

      if(codigo->label != NULL) {
         printf("%s:\n", codigo->label);
      }
      
      switch(codigo->operacao)
      {
         case NOP:
            printf("nop");
            break;
         case ADD:
            printf("add");
            break;
         case SUB:
            printf("sub");
            break;
         case MULT:
            printf("mult");
            break;
         case DIV:
            printf("div");
            break;
         case ADDI:
            printf("addi");
            break;
         case SUBI:
            printf("subi");
            break;
         case RSUBI:
            printf("rsubi");
            break;
         case MULTI:
            printf("multi");
            break;
         case DIVI:
            printf("divi");
            break;
         case RDIVI:
            printf("rdivi");
            break;
         case LSHIFT:
            printf("lshift");
            break;
         case LSHIFTI:
            printf("lshifti");
            break;
         case RSHIFT:
            printf("rshifti");
            break;
         case RSHIFTI:
            printf("rshifti");
            break;
         case AND:
            printf("and");
            break;
         case ANDI:
            printf("andi");
            break;
         case OR:
            printf("or");
            break;
         case ORI:
            printf("ori");
            break;
         case XOR:
            printf("xor");
            break;
         case XORI:
            printf("xori");
            break;
         case LOAD:
            printf("load");
            break;
         case LOADAI:
            printf("loadai");
            break;
         case LOADA0:
            printf("loada0");
            break;
         case CLOAD:
            printf("cload");
            break;
         case CLOADAI:
            printf("cloadai");
            break;
         case CLOADA0:
            printf("cloada0");
            break;
         case LOADI:
            printf("loadi");
            break;
         case STORE:
            printf("store");
            break;
         case STOREAI:
            printf("storeai");
            break;
         case STOREA0:
            printf("storea0");
            break;
         case CSTORE:
            printf("cstore");
            break;
         case CSTOREAI:
            printf("cstoreai");
            break;
         case CSTOREA0:
            printf("cstorea0");
            break;
         case I2I:
            printf("i2i");
            break;
         case C2C:
            printf("c2c");
            break;
         case C2I:
            printf("c2i");
            break;
         case I2C:
            printf("i2c");
            break;
         case CMP_LT:
            printf("cmp_lt");
            break;
         case CMP_LE:
            printf("cmp_le");
            break;
         case CMP_EQ:
            printf("cmp_eq");
            break;
         case CMP_GE:
            printf("cmp_ge");
            break;
         case CMP_GT:
            printf("cmp_gt");
            break;
         case CMP_NE:
            printf("cmp_ne");
            break;
         case CBR:
            printf("cbr");
            break;
         case JUMPI:
            printf("jumpi");
            break;
         case JUMP:
            printf("jump");
            break;
         default:
            break;
      }

      printf(" ");

      imprime_operandos(codigo->origem);

      printf(" => ");

      imprime_operandos(codigo->destino);

      printf("\n");

   }
}

void imprime_operandos(OperandoILOC *operando)
{
    if(operando==NULL) return;
    
    imprime_operando(operando);
    if(operando->proximo!=NULL) {
        printf(", ");
        imprime_operando(operando->proximo);
    }
}

void imprime_operando(OperandoILOC *operando)
{
    if(operando==NULL) return; 
        
    if(operando->tipo == IMEDIATO)
        printf("%i", operando->valor);
   //  else if(operando->tipo == REMENDO)
   //      printf("REMENDO");
    else printf("%s", operando->nome);

    if(operando->tipo == LABEL || operando->tipo == REMENDO)
      printf(" %p", operando);
}

void print_ILOC_intermed(char* str, CodigoILOC *codigo) {
   if(print_ILOC_intermed_global) {
		printf("\n>> OP: %s\n", str);
		imprime_codigo(codigo);
		printf("\n----------------------\n");
   }
}