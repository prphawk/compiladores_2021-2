#include "iloc_codigo.h"

#define FALSE 0
#define TRUE 1

int print_stuff = 1;

void _cria_codigo_append(Nodo *nodo, OperandoILOC *origem, OperacaoILOC operacao, OperandoILOC *destino) 
{
	CodigoILOC *codigo = _cria_codigo(origem, operacao, destino);
    _append(nodo, codigo);
}

void _cria_codigo_com_label_append(Nodo *nodo, char *label, OperandoILOC *origem, OperacaoILOC operacao, OperandoILOC *destino)
{
   CodigoILOC *codigo = _cria_codigo_com_label(label, origem, operacao, destino);
   _append(nodo, codigo);
}

void _append(Nodo *nodo, CodigoILOC *codigo_fim_ptr)
{
   if(codigo_fim_ptr == NULL) return;
   
   CodigoILOC *aux = codigo_fim_ptr; //fim_Codigo é o ponteiro que aponta para o FINAL de lista de codigo 
   while(aux->anterior != NULL) { //subo até o topo da instrução
      aux = aux->anterior;
   } //chega no inicio do codigo a ser adicionado
   aux->anterior = nodo->codigo; //no topo, ligo o fim do codigo do nodo com o inicio do outro nodo
   nodo->codigo = codigo_fim_ptr; //codigo inteiro no primeiro nodo
}

CodigoILOC *_append_codigo(CodigoILOC *codigo, CodigoILOC *codigo_head)
{
   if(codigo_head != NULL) {
   	codigo_head->anterior = codigo;
   }
   
   codigo = codigo_head;

   return codigo;
}

void _append_nodo(Nodo *pai, Nodo *filho) {

   CodigoILOC *filho_cod_ptr = filho->codigo;

   CodigoILOC *copia_filho_cod_ptr = copia_codigo(filho_cod_ptr);

   _append(pai, copia_filho_cod_ptr);
}

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

void remenda(Remendo *buracos, OperandoILOC *argamassa) {
	if(argamassa == NULL) return;

	Remendo *aux_buraco = buracos;

	while(aux_buraco != NULL) {
		OperandoILOC *operando = aux_buraco->operando;
		operando->tipo = argamassa->tipo;
		operando->nome = argamassa->nome;

		printf("\n>> remenda com ");
		imprime_operando(operando);

		aux_buraco = aux_buraco->proximo;
		//free(aux_buraco);
	}
}

void print_remendos(Remendo *buracos) {
	Remendo *aux_buraco = buracos;

	while(aux_buraco != NULL) {
	
		printf("\n>> print remendo ");
		imprime_operando(aux_buraco->operando);

		aux_buraco = aux_buraco->proximo;
		//free(aux_buraco);
	}
}

Remendo *append_remendo(Remendo *remendo_lst, OperandoILOC *remendo_operando) {
	if(remendo_operando == NULL) return remendo_lst;

	Remendo *remendo = cria_remendo(remendo_operando);

	if(remendo_lst == NULL) {
		remendo_lst = remendo;
	}
	else {
		remendo->proximo = remendo_lst;
		remendo_lst = remendo;
	}

	return remendo_lst;
}

Remendo *cria_remendo(OperandoILOC *remendo_operando) {
	Remendo *remendo = malloc(sizeof(Remendo));
	remendo->operando = remendo_operando;
	remendo->proximo = NULL;
	return remendo;
}

//#endregion Principais Código 

//#region Código 

//loadAI originRegister, originOffset => resultRegister // r3 = Memoria(r1 + c2)
void codigo_carrega_variavel(Nodo *nodo) {

	char* nome_malloc = gera_nome_registrador();

   DeslocamentoEscopo busca = busca_deslocamento_e_escopo(nodo->valor_lexico.label);

   int deslocamento = busca.deslocamento;

   OperandoILOC *origem_1_registrador = busca.eh_escopo_global ? reg_rbss() : reg_rfp();
   OperandoILOC *origem_2_deslocamento = gera_operando_imediato(deslocamento);

   OperandoILOC *destino = gera_operando_registrador(nome_malloc);

   _cria_codigo_append(nodo, lista(origem_1_registrador, origem_2_deslocamento), LOADAI, destino);

   nodo->reg_resultado = destino;

   free(nome_malloc);

	if(print_stuff) {
		printf("\n>> OP: carrega variavel\n");
		imprime_codigo(nodo->codigo);
		printf("\n----------------------\n");

   }
}

// loadI c1 => r2 // r2 = c1
void codigo_carrega_literal(Nodo *nodo) {

   if(!tem_valor_literal_int(nodo->valor_lexico)) return;

   CodigoILOC *codigo = instrucao_loadI(nodo->valor_lexico.valor_int, NULL);

   _append(nodo, codigo);
   
   nodo->reg_resultado = codigo->destino;

   if(print_stuff) {
		printf("\n>> OP: carrega literal\n");
		imprime_codigo(codigo);
   }
}

CodigoILOC *atribui_booleano(Nodo *expressao, char* rotulo_final) {

	char *destino_nome_malloc = gera_nome_registrador();
	OperandoILOC *destino = gera_operando_registrador(destino_nome_malloc);

	char *rotulo_true_malloc = gera_nome_rotulo();
	remenda(expressao->remendos_true, gera_operando_rotulo(rotulo_true_malloc));

	char *rotulo_false_malloc = gera_nome_rotulo();
	remenda(expressao->remendos_false, gera_operando_rotulo(rotulo_false_malloc));

	CodigoILOC *codigo_load_true = 	instrucao_loadI_reg(1, rotulo_true_malloc, destino);
	CodigoILOC *codigo_load_false = instrucao_loadI_reg(0, rotulo_false_malloc, copia_operando(destino));

	OperandoILOC *operando_rotulo_final = gera_operando_rotulo(rotulo_final);
	CodigoILOC *codigo_jump_rotulo_final = instrucao_jumpI(operando_rotulo_final);

	free(rotulo_true_malloc); free(rotulo_false_malloc); free(destino_nome_malloc);

	CodigoILOC *codigo_lst = NULL;
	codigo_lst = _append_codigo(codigo_lst, codigo_load_true); //se a expressao for true o remendado pula pra ca, atribui o booleano e depois pula pro final
	codigo_lst = _append_codigo(codigo_lst, codigo_jump_rotulo_final);
	codigo_lst = _append_codigo(codigo_lst, codigo_load_false);
	codigo_lst = _append_codigo(codigo_lst, copia_codigo(codigo_jump_rotulo_final));

	expressao->reg_resultado = destino;

	imprime_codigo(codigo_lst);

	return codigo_lst;
}


// storeAI r0 => r1 (rfp ou rbss), deslocamento // TODO: n passar o load da variavel
void codigo_atribuicao(Nodo *variavel, Nodo *atribuicao, Nodo *expressao) {

	char *rotulo_store = NULL;
	OperandoILOC *origem;
	DeslocamentoEscopo busca = busca_deslocamento_e_escopo(variavel->valor_lexico.label);
	OperandoILOC *destino_1_ponteiro = busca.eh_escopo_global ? reg_rbss() : reg_rfp();
	OperandoILOC *destino_2_deslocamento = gera_operando_imediato(busca.deslocamento);

	if(expressao->remendos_true != NULL || expressao->remendos_false != NULL) {
		rotulo_store = gera_nome_rotulo();
		CodigoILOC *codigo = atribui_booleano(expressao, rotulo_store);
		_append_nodo(atribuicao, expressao);
		_append(atribuicao, codigo);
	} else {
		_append_nodo(atribuicao, expressao);
	}
	origem = copia_operando(expressao->reg_resultado); //TODO CUIDAR!!!!!!!!!! TEM Q COPIAR SE FOR USAR EM NOVA INSTRUÇAO
	_cria_codigo_com_label_append(atribuicao, rotulo_store, origem, STOREAI, lista(destino_1_ponteiro, destino_2_deslocamento));
	
	//atribuicao->reg_resultado = destino_1_ponteiro; //precisa linkar o resultado da atribuição com esses dois regs? Acho q n pq atribuição não é uma expressão. entao n deve ter reg resultado.

   if(print_stuff) {
      printf("\n>> OP: codigo atribuicao\n");
      imprime_codigo(atribuicao->codigo);
	printf("\n----------------------\n");

   }
}

//Ex.: L1: add r1, r2 => r3
void codigo_expr_aritmetica(Nodo *esq, Nodo *operador, Nodo *dir) {

    char* op_nome_malloc = NULL;
    char *r1_nome_malloc = gera_nome_registrador();
    char *r2_nome_malloc = gera_nome_registrador();
    char *r3_nome_malloc = gera_nome_registrador();
    OperandoILOC *r1, *r2, *r3 = gera_operando_registrador(r3_nome_malloc);

    // if (esq->tem_remendo || dir->tem_remendo) {
    //     op_nome_malloc = gera_nome_rotulo();
	// }

    // codigo_avalia_expr(operador, esq, r1_nome_malloc, op_nome_malloc);
    // r1 = copia_operando(operador->reg_resultado);

    // codigo_avalia_expr(operador, dir, r2_nome_malloc, op_nome_malloc);
    // r2 = copia_operando(operador->reg_resultado);

    _cria_codigo_com_label_append(operador, op_nome_malloc, lista(r1, r2), operacao_iloc_binaria_nodo(operador), r3);
    
    operador->reg_resultado = r3;

   if(op_nome_malloc) free(op_nome_malloc); 
   free(r1_nome_malloc); free(r2_nome_malloc); free(r3_nome_malloc);

   if(print_stuff) {
      printf("\n>> OP: codigo expr aritmetica\n");
      imprime_codigo(operador->codigo);
   }
}

void codigo_expr_logica_relacional(Nodo *esq, Nodo *operador, Nodo *dir) {

	OperandoILOC *remendo_true = gera_operando_remendo();
	OperandoILOC *remendo_false = gera_operando_remendo();

    char *r3_nome_malloc = gera_nome_registrador();
    OperandoILOC *r1, *r2, *r3 = gera_operando_registrador(r3_nome_malloc);

	r1 = copia_operando(esq->reg_resultado);
    r2 = copia_operando(dir->reg_resultado);

    _append_nodo(operador, esq);
    _append_nodo(operador, dir);

	_cria_codigo_append(operador, lista(r1, r2), operacao_iloc_binaria_nodo(operador), r3);

	operador->reg_resultado = r3;

	CodigoILOC *codigo = codigo_compara_logico(copia_operando(r3), remendo_true, remendo_false);

	_append(operador, codigo);	

	operador->remendos_true = append_remendo(operador->remendos_true, remendo_true);

	// printf("\n-----------------------");
	// printf("\n>> remendo true aqui %p", remendo_true);
	// print_remendos(operador->remendos_true);

	operador->remendos_false = append_remendo(operador->remendos_false, remendo_false);

	// printf("\n-----------------------");
	// printf("\n>> remendo false aqui %p", remendo_false);
	// print_remendos(operador->remendos_false);
	// printf("\n-----------------------");


	free(r3_nome_malloc);
}

void codigo_expr_logica(Nodo *esq, Nodo *nodo_operador, Nodo *dir) {

	if(nodo_operador->tipo_operacao == nodo_and){
		codigo_expr_logica_and(esq, nodo_operador, dir);
	}
	else if(nodo_operador->tipo_operacao == nodo_or)
		codigo_expr_logica_or(esq, nodo_operador, dir);
	else
		codigo_expr_logica_relacional(esq, nodo_operador, dir);
}

void codigo_expr_logica_and(Nodo *esq, Nodo *operador, Nodo *dir) {

	char *rotulo_malloc = gera_nome_rotulo();

	print_remendos(esq->remendos_true);

	remenda(esq->remendos_true, gera_operando_rotulo(rotulo_malloc));

	print_remendos(esq->remendos_true);
	printf("\n");

	operador->remendos_true = dir->remendos_true;
	operador->remendos_false = concat_remendos(esq->remendos_false, dir->remendos_false);

	_append_nodo(operador, esq);
	_append(operador, instrucao_nop(rotulo_malloc));
    _append_nodo(operador, dir);

   free(rotulo_malloc);
}

void codigo_expr_logica_or(Nodo *esq, Nodo *operador, Nodo *dir) {

	char *rotulo_malloc = gera_nome_rotulo();

	print_remendos(esq->remendos_true);
	remenda(esq->remendos_false, gera_operando_rotulo(rotulo_malloc));

	print_remendos(esq->remendos_true);
	printf("\n");

	operador->remendos_false = dir->remendos_false;
	operador->remendos_true = concat_remendos(esq->remendos_true, dir->remendos_true);

	_append_nodo(operador, esq);
	_append(operador, instrucao_nop(rotulo_malloc));
    _append_nodo(operador, dir);

   free(rotulo_malloc);
}

void codigo_expr_logica_booleano(Nodo *nodo, int valor) {

	char *rotulo_malloc = gera_nome_rotulo();

	OperandoILOC *op_remendo = gera_operando_remendo();

   CodigoILOC *codigo = instrucao_jumpI(op_remendo);

   _append(nodo, codigo);

	if(valor) {
		nodo->remendos_true = append_remendo(nodo->remendos_true, op_remendo);
	} else {
		nodo->remendos_false = append_remendo(nodo->remendos_false, op_remendo);
	}

    free(rotulo_malloc);
}

void codigo_not(Nodo *operador, Nodo *expr) {

	operador->remendos_true = expr->remendos_false;
	operador->remendos_false =  expr->remendos_true;

	_append_nodo(operador, expr);
}

//dar um jeito nesse monte de repeticao de operadores no codigo inteiro.. 
int operacao_iloc_binaria_nodo(Nodo *nodo_operador) {
   switch(nodo_operador->tipo_operacao) {
      case nodo_EQ: return CMP_EQ; break;
      case nodo_NE: return CMP_NE; break;
      case nodo_LE: return CMP_LE; break;
      case nodo_GE: return CMP_GE; break;
      case nodo_GT: return CMP_GT; break;
      case nodo_LT: return CMP_LT; break;
      case nodo_add: return ADD; break;
      case nodo_sub: return SUB; break;
      case nodo_mult: return MULT; break;
      case nodo_div: return DIV; break;
      default: return NOP; break;
   }
}

void codigo_expr_unaria(Nodo *operador, Nodo *expr) {

	if(operador->tipo_operacao == nodo_sub) {
      codigo_sub(operador, expr);
   }
   else if(operador->tipo_operacao == nodo_not) {
      codigo_not(operador, expr);
   }
	else {
      _append_nodo(operador, expr);
		operador->reg_resultado = expr->reg_resultado;
    }
}

// load 0 -> r2
// cmp_NE r1, r2 -> r3 
// cbr r3 -> l2_true, l3_false // PC = endereço(l2) se r1 = true, senão PC = endereço(l3)
CodigoILOC *codigo_compara_logico(OperandoILOC *r1, OperandoILOC *op_label_true, OperandoILOC *op_label_false) {

	CodigoILOC *codigo_cmp_NE_0 = intrucoes_cmp_NE_0(r1);

	OperandoILOC *r3 = copia_operando(codigo_cmp_NE_0->destino);

	CodigoILOC *codigo_cbr = instrucao_cbr(r3, op_label_true, op_label_false);

	codigo_cbr->anterior = codigo_cmp_NE_0;

	return codigo_cbr;
}

// rsubI r1, 0 => r3 // r3 = 0 - r1
void codigo_sub(Nodo *operador, Nodo *expr) {

    //int nextInstructionLabel = this->getLabel();

    // resolveArithmetic(operador, expr, getRegister(), nextInstructionLabel); TODO curto ciruito, depende do BoolFLOW

   char* r3_nome_malloc = gera_nome_registrador();

	OperandoILOC *origem_1 = copia_operando(operador->reg_resultado);
	OperandoILOC *origem_2 = gera_operando_imediato(0);
	
	OperandoILOC *destino = gera_operando_registrador(r3_nome_malloc);

	_cria_codigo_append(operador, lista(origem_1, origem_2), RSUBI, destino);

   operador->reg_resultado = destino;

   free(r3_nome_malloc);
}
   
//#endregion Código 

//#region Instrução 

// cbr r1 -> l2_true, l3_false // PC = endereço(l2) se r1 = true, senão PC = endereço(l3)
CodigoILOC* instrucao_cbr(OperandoILOC *r1, OperandoILOC *op_label_true, OperandoILOC *op_label_false) {
   return _cria_codigo(r1, CBR, lista(op_label_true, op_label_false));
}

// load 0 -> r2
// cmp_NE r1, r2 -> r3 
CodigoILOC* intrucoes_cmp_NE_0(OperandoILOC *r1) {

   CodigoILOC *codigo_0 = instrucao_loadI(0, NULL);

    OperandoILOC *r2 = copia_operando(codigo_0->destino);
    char *r3_nome_malloc =  gera_nome_registrador();
    OperandoILOC *r3 = gera_operando_registrador(r3_nome_malloc);
    
    CodigoILOC *codigo_cmp_NE = _cria_codigo(lista(r1, r2) , CMP_NE, r3);

   free(r3_nome_malloc);
    codigo_cmp_NE->anterior = codigo_0;
    return codigo_cmp_NE;
}

CodigoILOC *instrucao_nop(char* label) {
   return _cria_codigo_com_label(label, NULL, NOP, NULL);
}

// loadI c1 => r2 // r2 = c1
CodigoILOC *instrucao_loadI(int valor, char *label) {

	char* nome_malloc = gera_nome_registrador();

   OperandoILOC *origem = gera_operando_imediato(valor);

   OperandoILOC *destino = gera_operando_registrador(nome_malloc);

   CodigoILOC *codigo = _cria_codigo_com_label(label, origem, LOADI, destino);

   free(nome_malloc);

   return codigo;
}

// loadI c1 => r2 // r2 = c1
CodigoILOC *instrucao_loadI_reg(int valor, char *label, OperandoILOC *r2) {

   OperandoILOC *origem = gera_operando_imediato(valor);

   CodigoILOC *codigo = _cria_codigo_com_label(label, origem, LOADI, r2);

   return codigo;
}

// ex: jumpI -> l1 // PC = endereço(l1)
CodigoILOC *instrucao_jumpI(OperandoILOC* destino) {

   return _cria_codigo(NULL, JUMPI, destino);
}

//#endregion Instrução 