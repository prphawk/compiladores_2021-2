#include "iloc.h"

int global_num_rotulos = 1;
int global_num_registradores = 1;

extern int print_ILOC_intermed_global;
extern Remendo *remendos_rotulo_funcao_global;

//#region Otimizacao

CodigoILOC* otimiza_ILOC(CodigoILOC* codigo) {
   CodigoILOC* codigo_lst = codigo;
   CodigoILOC* codigo_anterior = NULL;

   while(codigo_lst != NULL) {

      if(codigo_lst->operacao == NOP) {
         codigo_lst = nops(codigo_anterior, codigo_lst);
      }
      else if(codigo_lst->operacao == LOADI && codigo_lst->destino->tipo == REGISTRADOR) {
         imediatos_comuns(codigo_lst);
      }
      else if(codigo_lst->operacao == JUMP) {
         codigo_morto_jump(codigo_lst);
      }
      else if(codigo_lst->operacao == STOREAI && codigo_lst->destino->tipo == REGISTRADOR_PONTEIRO) {
         propag_copias(codigo_lst);
      }
      otimiza_ILOC_janela(codigo_lst);

      codigo_anterior = codigo_lst;
      codigo_lst = codigo_lst->proximo;
   }

   return codigo;
}

int origens_iguais(CodigoILOC *cod1, CodigoILOC *cod2) {

   if(eq_reg(cod1->origem, cod2->origem)) {

      if(!cod1->origem->proximo && !cod2->origem->proximo) return 1;

      else return eq_reg(cod1->origem->proximo, cod2->origem->proximo);
   } return 0;
}

void otimiza_ILOC_janela(CodigoILOC* cod_ref) {

   // int janela_atual = 0;
   // int janela = 2;
   // int mudou = 1;

   CodigoILOC* cod_atual = cod_ref->proximo;
   CodigoILOC* cod_anterior = cod_ref;

   // while(mudou) {

   //    mudou = 0;
      while(cod_atual != NULL) {
         
         //while(cod_atual != NULL && janela_atual < janela) {

            // se tiver entrada de laço ou desvio ou se for instrução modificando o valor do reg_ponteiro
            if(cod_atual->label != NULL || eh_desvio(cod_atual)) return;


            if(cod_atual->operacao != cod_ref->operacao) break;

            if(eq_reg(cod_atual->destino, cod_ref->destino)) {
               
               if(!origens_iguais(cod_atual, cod_ref)) return; //ta mexendo no destino, deixa quieto

               cod_atual = deleta_instrucao_atual(cod_anterior); //eh igual
               //mudou = 1;
            }
            cod_anterior = cod_atual;
            cod_atual = cod_atual->proximo;
            //janela_atual++;
         }
         //janela_atual = 0;
      //}
      // cod_ref = codigo;
      // cod_atual = codigo->proximo;
      // cod_anterior = codigo;
      // janela += 2;
}

/*
storeAI r17 => rfp, 24
loadAI rfp, 24 => r18
storeAI r18 => rfp, 16
*/
void propag_copias(CodigoILOC *cod_ref) {

   OperandoILOC* op_ponteiro = cod_ref->destino; 

   CodigoILOC* cod_atual = cod_ref->proximo;
   CodigoILOC* cod_anterior = cod_ref;
   
   while(cod_atual != NULL) {

      // se tiver entrada de laço ou desvio ou se for instrução modificando o valor do reg_ponteiro
      if(cod_atual->label != NULL || eh_desvio(cod_atual) || eq_reg_ptr(cod_atual->destino, op_ponteiro)) {
         return;
      }
      if(cod_atual->operacao == LOADAI && eq_reg_ptr(cod_atual->origem, op_ponteiro)) { // rfp, 24 == rfp, 24
         OperandoILOC* op_reg_original = cod_atual->destino; // r18
         printf("\n>> entrou");
         print_operando(op_reg_original);
         substitui_operando(cod_atual->proximo, op_reg_original, cod_ref->origem); // r18 (vira)-> r17
         cod_atual = deleta_instrucao_atual(cod_anterior);
      }
      cod_anterior = cod_atual;
      cod_atual = cod_atual->proximo;
   }
}

CodigoILOC* nops(CodigoILOC *cod_anterior, CodigoILOC *codigo) {
   CodigoILOC* cod_atual = codigo;
   
   if(cod_anterior == NULL) return codigo;
   if(cod_atual->label == NULL) return codigo;
   if(cod_atual->proximo == NULL || cod_atual->proximo->label != NULL) return codigo;
   
   cod_atual->proximo->label = copia_nome(cod_atual->label);
   return deleta_instrucao_atual(cod_anterior);
}

void codigo_morto_jump(CodigoILOC *codigo) {
   CodigoILOC* cod_atual = codigo->proximo;
   CodigoILOC* cod_anterior = codigo;
   
   while(cod_atual != NULL) {
      if(cod_atual->label == NULL) {
         cod_atual = deleta_instrucao_atual(cod_anterior);
      } else break;
      cod_anterior = cod_atual;
      cod_atual = cod_atual->proximo;
   }
}

// substitui novos registradores de valores imediatos com registradores já existentes que possuem mesmo valor
void imediatos_comuns(CodigoILOC* cod_ref) {

   if(cod_ref == NULL) return;

   CodigoILOC* cod_atual = cod_ref->proximo;
   CodigoILOC* cod_anterior = cod_ref;

   while(cod_atual != NULL) {

      if(cod_atual->operacao == LOADI && cod_atual->destino->tipo == REGISTRADOR) {

         int regs_diferentes = !eq_reg(cod_atual->destino, cod_ref->destino);
         int imediatos_iguais = cod_atual->origem->valor == cod_ref->origem->valor;

         if(imediatos_iguais && regs_diferentes) {
            substitui_operando(cod_atual->proximo, cod_atual->destino, cod_ref->destino);
            cod_atual = deleta_instrucao_atual(cod_anterior);
         }
      }

      cod_atual = simplif_algebrica(cod_ref, cod_atual, cod_anterior);

      cod_anterior = cod_atual;
      cod_atual = cod_atual->proximo;
   }
}

// se r2 eh registrador de imediato c1, substitui ADD r1, r2 -> r3 por ADDI r1, c1 -> r3
CodigoILOC* simplif_algebrica(CodigoILOC* cod_ref, CodigoILOC *cod_atual, CodigoILOC *cod_anterior) {
   
   if(!(cod_atual && cod_atual->origem)) return cod_atual;
    
   OperandoILOC* operando_original = cod_atual->origem->proximo;
   if(!eq_reg(operando_original, cod_ref->destino)) return cod_atual;

   int valor = cod_ref->origem->valor;

    switch (cod_atual->operacao)
    {
      case ADD:
         if(valor == 0) {
            substitui_operando(cod_atual->proximo, cod_atual->destino, cod_atual->origem);
            return deleta_instrucao_atual(cod_anterior);
         }
         cod_atual->operacao = ADDI;
         vira_imediato(operando_original, valor);
         break;
      case SUB:
         if(valor == 0) {
            substitui_operando(cod_atual->proximo, cod_atual->destino, cod_atual->origem);
            return deleta_instrucao_atual(cod_anterior);
         }
         cod_atual->operacao = SUBI;
         vira_imediato(operando_original, valor);
         break;
      case MULT:
         if(valor == 1) {
            substitui_operando(cod_atual->proximo, cod_atual->destino, cod_atual->origem);
            return deleta_instrucao_atual(cod_anterior);
         }
         cod_atual->operacao = MULTI;
         vira_imediato(operando_original, valor);
         break;
      case DIV:
         if(valor == 1) {
            substitui_operando(cod_atual->proximo, cod_atual->destino, cod_atual->origem);
            return deleta_instrucao_atual(cod_anterior);
         }
         cod_atual->operacao = DIVI;
         vira_imediato(operando_original, valor);
         break;
      
      default: break;
    }
   return cod_atual;
}

void vira_imediato(OperandoILOC* operando, int valor) {
   libera_nome(operando->nome);
   operando->nome = NULL;
   operando->tipo = IMEDIATO;
   operando->valor = valor;
}

//so ate achar um, nao vai td lista
void substitui_operando(CodigoILOC *codigo, OperandoILOC *original, OperandoILOC *sub) {

   CodigoILOC *aux_codigo = codigo;

   while(aux_codigo != NULL) {

      OperandoILOC* operando = aux_codigo->origem;

      while (operando != NULL) // eu sei q sao so dois mas n queria repitir o compare....
      {
         if(eq_str(operando->nome , original->nome)) {

            libera_nome(operando->nome);
            operando->nome = copia_nome(sub->nome);
            operando->tipo = sub->tipo;
            return;
         }
         operando = operando->proximo;
      }
      aux_codigo = aux_codigo->proximo;
   }
}

CodigoILOC* deleta_instrucao_atual(CodigoILOC* codigo_anterior) {
   CodigoILOC* codigo_deletar = codigo_anterior->proximo;

   if(codigo_deletar != NULL) {
      codigo_anterior->proximo = codigo_deletar->proximo;
      libera_head_codigo(codigo_deletar);
   }

   return codigo_anterior;
}
//#endregion Otimizacao

//#region Aux

int eh_desvio(CodigoILOC *codigo) {
   return codigo->operacao == JUMP || codigo->operacao == JUMPI;
}

int eq_str(char* str1, char* str2) {
   if(str1 == NULL || str2 == NULL) return 0;
   return !strcmp(str1, str2);
}

int eq_reg(OperandoILOC* dest1, OperandoILOC* dest2) {
   if(dest1 == NULL || dest2 == NULL) return 0;
   if(dest1->tipo != dest2->tipo) return 0;
   if(dest1->tipo == IMEDIATO) return dest1->valor == dest2->valor;
   return eq_str(dest1->nome, dest2->nome);
}
int eq_reg_ptr(OperandoILOC* dest1, OperandoILOC* dest2) {
   if(dest1 == NULL || dest2 == NULL) return 0;
   if(dest1->tipo != REGISTRADOR_PONTEIRO) return 0;
   if(dest1->proximo == NULL || dest2->proximo == NULL) return 0;
   return eq_reg(dest1, dest2) && dest1->proximo->valor == dest2->proximo->valor;
}

CodigoILOC* reverte(CodigoILOC* head) {

    if(head == NULL || head->proximo == NULL) return head;

    CodigoILOC* rest = reverte(head->proximo);

    head->proximo->proximo = head;

    head->proximo = NULL;

    return rest;
}

int conta_instrucoes(CodigoILOC *codigo) {
	CodigoILOC *aux = codigo;
	int count = 0;
	while(aux != NULL) {
		aux = aux->proximo;
		count++;
	}
	return count;
}
//#endregion Aux

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

OperandoILOC *gera_operando_rotulo_ponteiro(char** nome_ptr) {
   return _cria_operando_rotulo_ponteiro(nome_ptr);
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
   operando->nome_ptr = NULL;
   operando->valor = valor;
   operando->tipo = tipo;
   operando->proximo = NULL;
   return operando;
}

OperandoILOC *_cria_operando_rotulo_ponteiro(char** nome_ptr) {
   OperandoILOC *operando = malloc(sizeof(OperandoILOC));
   operando->nome = NULL;
   operando->nome_ptr = nome_ptr;
   operando->valor = 0;
   operando->tipo = ROTULO_PONTEIRO;
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
    codigo->proximo = NULL;

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
	copia->proximo = copia_codigo(codigo->proximo);

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
	copia->proximo = copia_codigo_repassa_remendo(lst_true, lst_false, codigo->proximo);

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
      substitui_remendo(remendos_rotulo_funcao_global, operando, copia);
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

void libera_head_codigo(CodigoILOC *codigo) {
    if(codigo == NULL) return;

    libera_nome(codigo->label);
    codigo->label = NULL;

    libera_operando(codigo->origem);
    libera_operando(codigo->destino);

    free(codigo);
}

void libera_codigo(CodigoILOC *codigo) {

   if(codigo == NULL) return;

    libera_codigo(codigo->proximo);

    libera_head_codigo(codigo);
}

void libera_nome(char *nome) {
   if(nome != NULL) free(nome);
}

void libera_operando(OperandoILOC *operando) {
    if(operando == NULL) return;

    libera_operando(operando->proximo);

   if(operando->tipo != REGISTRADOR_PONTEIRO && operando->tipo != ROTULO_PONTEIRO) {
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
      if(codigo->label != NULL) {
         printf("%s: ", codigo->label);
      }
      
      switch(codigo->operacao)
      {
         case HALT: printf("halt\n"); return print_codigo(codigo->proximo); break;
         case NOP: printf("nop\n"); return print_codigo(codigo->proximo); break;
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
         case LOADAO: printf("loadAO"); break;
         case CLOAD: printf("cload"); break;
         case CLOADAI: printf("cloadAI"); break;
         case CLOADAO: printf("cloadAO"); break;
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

      print_codigo(codigo->proximo);

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
    else if(operando->tipo == ROTULO_PONTEIRO && operando->nome_ptr)
        printf("%s", *(operando->nome_ptr));
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
