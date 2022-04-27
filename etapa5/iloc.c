#include "iloc.h"

int global_num_rotulos = 1;
int global_num_registradores = 1;

extern int print_ILOC_intermed_global;

int conta_instrucoes(CodigoILOC *codigo) {
	CodigoILOC *aux = codigo;
	int count = 0;
	while(aux != NULL) {
		aux = aux->anterior;
		count++;
	}
	return count;
}

//#region Cria

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

OperandoILOC *_cria_operando(char* nome, int valor, TipoOperando tipo) {
    OperandoILOC *operando = malloc(sizeof(OperandoILOC));
    operando->nome = nome;
    operando->valor = valor;
    operando->tipo = tipo;
    operando->proximo = NULL;
    return operando;
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

OperandoILOC *lista(OperandoILOC *primeiro, OperandoILOC *segundo) 
{
    primeiro->proximo = segundo;
    return primeiro;
}

//#endregion Cria

//#region Copia

char* copia_nome(char *nome) {
   if(nome == NULL) return NULL;
   return strdup(nome);
}

char* copia_nome_operando(char *nome, TipoOperando tipo) {
   if(tipo == REGISTRADOR_PONTEIRO) return nome;
   return copia_nome(nome);
}

OperandoILOC *copia_operando(OperandoILOC *operando) {

   if(operando == NULL) return NULL;

    OperandoILOC *copia = malloc(sizeof(OperandoILOC));
    copia->nome = copia_nome_operando(operando->nome, operando->tipo);
    copia->valor = operando->valor;
    copia->tipo = operando->tipo;
    copia->proximo = copia_operando(operando->proximo);
    return copia;
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

/* ao fazer deep copy do codigo de um nodo para outro, o endereço do operando que estava na lista de remendos acaba 
apontando pra um operando não mAIs utilizado no código final. atualizamos o endereço do remendo com o novo endereço 
do resultado de deep copy para que a função remenda() consiga encontrar o operando correto. */
CodigoILOC *copia_codigo_repassa_remendo(Remendo *lst_true, Remendo *lst_false, CodigoILOC *codigo) {

	if(codigo == NULL) return NULL;

	CodigoILOC *copia = malloc(sizeof(CodigoILOC));
	copia->label = copia_nome(codigo->label);
	copia->origem = copia_operando_repassa_remendo(lst_true, lst_false, codigo->origem);
	copia->operacao = codigo->operacao;
	copia->destino = copia_operando_repassa_remendo(lst_true, lst_false, codigo->destino);
	copia->anterior = copia_codigo_repassa_remendo(lst_true, lst_false, codigo->anterior);

   return copia;
}

OperandoILOC *copia_operando_repassa_remendo(Remendo *lst_true, Remendo *lst_false, OperandoILOC *operando) {

   if(operando == NULL) return NULL;
 
    OperandoILOC *copia = malloc(sizeof(OperandoILOC));
    copia->nome = copia_nome_operando(operando->nome, operando->tipo);
    copia->valor = operando->valor;
    copia->tipo = operando->tipo;

   if(operando->tipo == REMENDO) {
      substitui_remendo(lst_true, operando, copia);
      substitui_remendo(lst_false, operando, copia);
   }

    copia->proximo = copia_operando_repassa_remendo(lst_true, lst_false, operando->proximo);

    return copia;
}

Remendo *substitui_remendo(Remendo *lst, OperandoILOC *velho, OperandoILOC *novo) {
   Remendo *aux = lst;
   
   while(aux != NULL) {
      if(aux->operando == velho) {
         aux->operando = novo;
      }
      aux = aux->proximo;
   }

   return lst;
}

//#endregion Copia

//#region Remendos

Remendo *concat_remendos(Remendo *lst1, Remendo *lst2) {
	if(lst1 == NULL) return lst2;
	if(lst2 == NULL) return lst1;

	Remendo *ultimo_lst1 = lst1;
	while(ultimo_lst1->proximo != NULL) {
		ultimo_lst1 = ultimo_lst1->proximo;
	}
	ultimo_lst1->proximo = lst2;
	return lst1;
}

Remendo *remenda(Remendo *buraco_lst, OperandoILOC *argamassa) {
	if(argamassa == NULL) return buraco_lst;

	Remendo *aux_buraco;

	while(buraco_lst != NULL) {
		OperandoILOC *operando = buraco_lst->operando;

		operando->tipo = argamassa->tipo;
		operando->nome = copia_nome_operando(argamassa->nome, argamassa->tipo);
		
		aux_buraco = buraco_lst;
		buraco_lst = buraco_lst->proximo;

		libera_head_remendo(aux_buraco); //menos o operando. ele é liberado pelo codigo de algm.
	}

	libera_operando(argamassa);
	return buraco_lst;
}

Remendo *append_remendo(Remendo *remendo_lst, OperandoILOC *remendo_operando) {
	if(remendo_operando == NULL) return remendo_lst;

	Remendo *remendo = cria_remendo();
	remendo->operando = remendo_operando;

	if(remendo_lst == NULL) {
		remendo_lst = remendo;
	}
	else {
		remendo->proximo = remendo_lst;
		remendo_lst = remendo;
	}

	return remendo_lst;
}

Remendo *cria_remendo() {
	Remendo *remendo = malloc(sizeof(Remendo));
	remendo->operando = NULL;
	remendo->proximo = NULL;
	return remendo;
}

//#endregion Remendos

//#region Libera

void libera_codigo(CodigoILOC *codigo) {
    if(codigo == NULL) return;

    libera_codigo(codigo->anterior);

    libera_nome(codigo->label);
    codigo->label = NULL;

    libera_operando(codigo->origem);
    libera_operando(codigo->destino);

    free(codigo);
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
}

void libera_head_remendo(Remendo *remendo) {
   if(remendo == NULL) return;

   free(remendo);
}
//#endregion Libera

//#region Prints

void print_codigo(CodigoILOC *codigo)
{
   if(codigo!=NULL)
   {
      print_codigo(codigo->anterior);

      if(codigo->label != NULL) {
         printf("%s: ", codigo->label);
      }
      
      switch(codigo->operacao)
      {
         case HALT: printf("halt\n"); return; break;
         case NOP: printf("nop\n"); return; break;
         case ADD: printf("add"); break;
         case SUB: printf("sub"); break;
         case MULT: printf("mult"); break;
         case DIV: printf("div"); break;
         case ADDI: printf("addI"); break;
         case SUBI: printf("subI"); break;
         case RSUBI: printf("rsubI"); break;
         case MULTI: printf("multI"); break;
         case DIVI: printf("divI"); break;
         case RDIVI: printf("rdivI"); break;
         case LSHIFT: printf("lshift"); break;
         case LSHIFTI: printf("lshiftI"); break;
         case RSHIFT: printf("rshiftI"); break;
         case RSHIFTI: printf("rshiftI"); break;
         case AND: printf("and"); break;
         case ANDI: printf("andI"); break;
         case OR: printf("or"); break;
         case ORI: printf("orI"); break;
         case XOR: printf("xor"); break;
         case XORI: printf("xorI"); break;
         case LOAD: printf("load"); break;
         case LOADAI: printf("loadAI"); break;
         case LOADA0: printf("loadA0"); break;
         case CLOAD: printf("cload"); break;
         case CLOADAI: printf("cloadAI"); break;
         case CLOADA0: printf("cloadA0"); break;
         case LOADI: printf("loadI"); break;
         case STORE: printf("store"); break;
         case STOREAI: printf("storeAI"); break;
         case STOREAO: printf("storeAO"); break;
         case CSTORE: printf("cstore"); break;
         case CSTOREAI: printf("cstoreAI"); break;
         case CSTOREAO: printf("cstoreAO"); break;
         case I2I: printf("i2i"); break;
         case C2C: printf("c2c"); break;
         case C2I: printf("c2i"); break;
         case I2C: printf("i2c"); break;
         case CMP_LT: printf("cmp_LT"); break;
         case CMP_LE: printf("cmp_LE"); break;
         case CMP_EQ: printf("cmp_EQ"); break;
         case CMP_GE: printf("cmp_GE"); break;
         case CMP_GT: printf("cmp_GT"); break;
         case CMP_NE: printf("cmp_NE"); break;
         case CBR: printf("cbr"); break;
         case JUMPI: printf("jumpI"); break;
         case JUMP: printf("jump"); break;
         default: break;
      }

      printf(" ");

      print_operandos(codigo->origem);

      printf(" => ");

      print_operandos(codigo->destino);

      printf("\n");

   }
}

void print_operandos(OperandoILOC *operando)
{
    if(operando==NULL) return;
    
    print_operando(operando);
    if(operando->proximo!=NULL) {
        printf(", ");
        print_operando(operando->proximo);
    }
}

void print_operando(OperandoILOC *operando)
{
    if(operando==NULL) return; 
        
    if(operando->tipo == IMEDIATO)
        printf("%i", operando->valor);
    else if(operando->tipo == REMENDO)
        printf("REMENDO");
    else printf("%s", operando->nome);

    if(operando->tipo == REMENDO)
      printf(" %p", operando);
}

void print_remendos(Remendo *remendo_lst) {
   if(!print_ILOC_intermed_global) return;

   Remendo *aux = remendo_lst;
   printf("\nImprimindo lista de remendos");
   while(aux != NULL) {
      printf("\n Remendo:");
      print_operando(aux->operando);
      aux = aux->proximo;
   }
   printf("\nFim.");
}

void print_ILOC_intermed(char* str, CodigoILOC *codigo) {
   if(print_ILOC_intermed_global) {
		printf("\n>> OP: %s\n", str);
		print_codigo(codigo);
		printf("\n----------------------\n");
   }
}

//#endregion Prints
