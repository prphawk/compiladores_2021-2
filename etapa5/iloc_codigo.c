#include "iloc_codigo.h"

#define FALSE 0
#define TRUE 1

extern int print_ILOC_intermed_global;

//#region Auxiliares 

CodigoILOC *_append_codigo(CodigoILOC *lst, CodigoILOC *new_lst)
{
	if(new_lst == NULL) return lst;

	CodigoILOC *aux_new_lst = new_lst; //new_lst é o ponteiro que aponta para o FINAL de lista de codigo 
	while(aux_new_lst->anterior != NULL) { //subo até o topo da instrução
		aux_new_lst = aux_new_lst->anterior;
	} //chega no inicio do codigo a ser adicionado
	aux_new_lst->anterior = lst; //no topo, ligo o fim do codigo do nodo com o inicio do outro nodo
	lst = new_lst; //codigo inteiro no primeiro nodo

	return lst;
}

void _append(Nodo *nodo, CodigoILOC *codigo_fim_ptr)
{
   nodo->codigo = _append_codigo(nodo->codigo, codigo_fim_ptr);
}

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

void codigo_append_nodo(Nodo *pai, Nodo *filho) {

	if(pai == NULL || filho == NULL) return;

   CodigoILOC *copia_filho_cod_ptr = copia_codigo_repassa_remendo(filho->remendos_true, filho->remendos_false, filho->codigo);

   _append(pai, copia_filho_cod_ptr);
}

int tem_buracos(Nodo *nodo) {
	if(nodo != NULL) {
		if(nodo->remendos_false != NULL || nodo->remendos_true != NULL) 
			return TRUE;
	}

	return FALSE;
}

//#endregion Auxiliares 

//#region Código 

//loadAI originRegister, originOffset => resultRegister // r3 = Memoria(r1 + c2)
void codigo_carrega_variavel(Nodo *nodo) {

   DeslocamentoEscopo busca = busca_deslocamento_e_escopo(nodo->valor_lexico.label);

   int deslocamento = busca.deslocamento;

   OperandoILOC *origem_1_registrador = busca.eh_escopo_global ? reg_rbss() : reg_rfp();
   OperandoILOC *origem_2_deslocamento = gera_operando_imediato(deslocamento);

   OperandoILOC *destino = gera_operando_registrador(gera_nome_registrador());

   _cria_codigo_append(nodo, lista(origem_1_registrador, origem_2_deslocamento), LOADAI, destino);

   nodo->reg_resultado = destino;

	print_ILOC_intermed("Carrega variavel", nodo->codigo);
}

// loadI c1 => r2 // r2 = c1
void codigo_carrega_literal(Nodo *nodo) {

   if(!tem_valor_literal_int(nodo->valor_lexico)) return;

   CodigoILOC *codigo = instrucao_loadI(nodo->valor_lexico.valor_int, NULL);

   _append(nodo, codigo);
   
   nodo->reg_resultado = codigo->destino;

	print_ILOC_intermed("Carrega literal", codigo);
}

CodigoILOC *atribui_booleano(Nodo *expressao, char* rotulo_final) {

	OperandoILOC *destino = gera_operando_registrador(gera_nome_registrador());

	char *rotulo_true = gera_nome_rotulo();
	char *rotulo_false = gera_nome_rotulo();
	
	char *rotulo_true_copia = copia_nome(rotulo_true); //pq o operando q remenda eh liberado dps do remendo mas ainda precisa do rotulo
	char *rotulo_false_copia = copia_nome(rotulo_false);

	OperandoILOC *op_remendo_true = gera_operando_rotulo(rotulo_true);
	OperandoILOC *op_remendo_false = gera_operando_rotulo(rotulo_false);

	remenda(expressao->remendos_true, op_remendo_true);
	remenda(expressao->remendos_false, op_remendo_false);

	CodigoILOC *codigo_load_true 	= instrucao_loadI_reg(1, rotulo_true_copia, destino);
	CodigoILOC *codigo_load_false = instrucao_loadI_reg(0, rotulo_false_copia, copia_operando(destino));

	OperandoILOC *operando_rotulo_final = gera_operando_rotulo(rotulo_final);
	CodigoILOC *codigo_jump_rotulo_final = instrucao_jumpI(operando_rotulo_final);

	CodigoILOC *codigo_jump_rotulo_final_copia = copia_codigo(codigo_jump_rotulo_final);

	CodigoILOC *codigo_lst = NULL;
	codigo_lst = _append_codigo(codigo_lst, codigo_load_true); //se a expressao for true o remendado pula pra ca, atribui o booleano e depois pula pro final
	codigo_lst = _append_codigo(codigo_lst, codigo_jump_rotulo_final);
	codigo_lst = _append_codigo(codigo_lst, codigo_load_false);
	codigo_lst = _append_codigo(codigo_lst, codigo_jump_rotulo_final_copia);

	expressao->reg_resultado = destino;

	return codigo_lst;
}

/*
S → while { B.f=S.next; B.t=rot(); }
	(B) { S.begin=rot(); S1.next=S.begin; }
	S1 { S.code=gera(S.begin:) || B.code ||
	gera(B.t:) || S1.code || gera(goto S.begin) }
*/
void codigo_while(Nodo *nodo, Nodo *expressao, Nodo *bloco) {
	char *rotulo_expressao 	= gera_nome_rotulo();
	char *rotulo_bloco		= gera_nome_rotulo();
	char *rotulo_fim 			= gera_nome_rotulo();

	CodigoILOC *codigo_nop_expressao 	= instrucao_nop(copia_nome(rotulo_expressao));
	CodigoILOC *codigo_nop_bloco 			= instrucao_nop(copia_nome(rotulo_bloco));
	CodigoILOC *codigo_nop_fim 			= instrucao_nop(copia_nome(rotulo_fim));

	converte_para_logica(expressao);

	remenda(expressao->remendos_true, gera_operando_rotulo(rotulo_bloco));
	remenda(expressao->remendos_false, gera_operando_rotulo(rotulo_fim));

	CodigoILOC *codigo_jump_expressao 	= instrucao_jumpI(gera_operando_rotulo(rotulo_expressao));

	_append(nodo, codigo_nop_expressao);
	codigo_append_nodo(nodo, expressao);
	_append(nodo, codigo_nop_bloco);
	codigo_append_nodo(nodo, bloco);
	_append(nodo, codigo_jump_expressao);
	_append(nodo, codigo_nop_fim);	

	print_ILOC_intermed("Codigo while", nodo->codigo);
}
//comando_iterativo: TK_PR_FOR '(' comando_atribuicao ':' expressao ':' comando_atribuicao')' bloco_comandos
void codigo_for(Nodo *nodo, Nodo *atribuicao_inicial, Nodo *expressao, Nodo *atribuicao_final, Nodo *bloco) {
	char *rotulo_expressao 	= gera_nome_rotulo();
	char *rotulo_bloco		= gera_nome_rotulo();
	char *rotulo_fim 			= gera_nome_rotulo();

	CodigoILOC *codigo_nop_expressao 	= instrucao_nop(copia_nome(rotulo_expressao));
	CodigoILOC *codigo_nop_bloco 			= instrucao_nop(copia_nome(rotulo_bloco));
	CodigoILOC *codigo_nop_fim 			= instrucao_nop(copia_nome(rotulo_fim));

	converte_para_logica(expressao);

	remenda(expressao->remendos_true, gera_operando_rotulo(rotulo_bloco));
	remenda(expressao->remendos_false, gera_operando_rotulo(rotulo_fim));

	CodigoILOC *codigo_jump_expressao 	= instrucao_jumpI(gera_operando_rotulo(rotulo_expressao));

	codigo_append_nodo(nodo, atribuicao_inicial);
	_append(nodo, codigo_nop_expressao);
	codigo_append_nodo(nodo, expressao);
	_append(nodo, codigo_nop_bloco);
	codigo_append_nodo(nodo, bloco);
	codigo_append_nodo(nodo, atribuicao_final);
	_append(nodo, codigo_jump_expressao);
	_append(nodo, codigo_nop_fim);	

	print_ILOC_intermed("Codigo for", nodo->codigo);
}

void converte_para_logica(Nodo *expressao) {
	if(tem_buracos(expressao) || !expressao->reg_resultado) return;

	OperandoILOC *op_remendo_true = gera_operando_remendo();
	OperandoILOC *op_remendo_false = gera_operando_remendo();

	OperandoILOC *reg_resultado = copia_operando(expressao->reg_resultado);

	CodigoILOC *codigo = codigo_compara_logico(reg_resultado, op_remendo_true, op_remendo_false);

	_append(expressao, codigo);

	expressao->remendos_true = append_remendo(expressao->remendos_true, op_remendo_true);
	expressao->remendos_false = append_remendo(expressao->remendos_false, op_remendo_false);
}
/*
S -> 	if { B.t=rot(); B.f=rot(); }
		(B) { S1 .next=S.next; }
		S1 else { S2.next=S.next; }
		S2 { S.code=B.code || gera(B.t:) || S1.code ||
		gera(goto S.next) || gera(B.f:); || S2.code }
*/
void codigo_if_else(Nodo *nodo, Nodo *expressao, Nodo *bloco_true, Nodo *bloco_false) {
	
	//eu não tenho TEMPO pra refatorar isso aqui, bear with me:

	char *rotulo_fim 			= gera_nome_rotulo();
	char *rotulo_true 		= gera_nome_rotulo();
	char *rotulo_false 		= NULL;
	CodigoILOC *codigo_nop_false		 = NULL;
	CodigoILOC *codigo_jump_fim_copia = NULL;

	converte_para_logica(expressao);

	expressao->remendos_true 		= remenda(expressao->remendos_true, gera_operando_rotulo(copia_nome(rotulo_true)));

	CodigoILOC *codigo_nop_true 	= instrucao_nop(rotulo_true);
	CodigoILOC *codigo_jump_fim 	= instrucao_jumpI(gera_operando_rotulo(copia_nome(rotulo_fim)));

	if(bloco_false != NULL) {
		rotulo_false = gera_nome_rotulo();
		codigo_nop_false 	= instrucao_nop(copia_nome(rotulo_false));

		codigo_jump_fim_copia = copia_codigo(codigo_jump_fim);
	} else {
		rotulo_false = copia_nome(rotulo_fim);
	}

	expressao->remendos_false		 = remenda(expressao->remendos_false, gera_operando_rotulo(rotulo_false));

	CodigoILOC *codigo_nop_fim 	 = instrucao_nop(rotulo_fim);

	codigo_append_nodo(nodo, expressao);
	_append(nodo, codigo_nop_true);
	codigo_append_nodo(nodo, bloco_true);
	_append(nodo, codigo_jump_fim);

	if(bloco_false != NULL) {
		_append(nodo, codigo_nop_false);
		codigo_append_nodo(nodo, bloco_false);
		_append(nodo, codigo_jump_fim_copia);
	}

	_append(nodo, codigo_nop_fim);

	print_ILOC_intermed("Codigo if else", nodo->codigo);
}

void codigo_chamada_funcao(Nodo *nodo, char *nome_funcao, Nodo *lista_argumentos) {

	remenda_argumentos_chamada_funcao(nodo, lista_argumentos);
	
}

void remenda_argumentos_chamada_funcao(Nodo *chamada_funcao, Nodo *lista_argumentos) {
	Nodo *arg_lst = lista_argumentos;

	while(arg_lst != NULL) {

		if(tem_buracos(arg_lst)) {
			char *rotulo_fim_arg = gera_nome_rotulo();
			CodigoILOC *codigo = atribui_booleano(arg_lst, rotulo_fim_arg);
			codigo_append_nodo(chamada_funcao, arg_lst);
			_append(chamada_funcao, codigo);

			CodigoILOC *nop_fim_arg = instrucao_nop(copia_nome(rotulo_fim_arg));
			_append(chamada_funcao, nop_fim_arg);
		} else {
			codigo_append_nodo(chamada_funcao, arg_lst);
		}
		arg_lst = arg_lst->irmao;
	}	
}


// storeAI r0 => r1 (rfp ou rbss), deslocamento // TODO: n passar o load da variavel
void codigo_atribuicao(Nodo *variavel, Nodo *atribuicao, Nodo *expressao) {

	char *rotulo_store = NULL;
	OperandoILOC *origem;
	DeslocamentoEscopo busca = busca_deslocamento_e_escopo(variavel->valor_lexico.label);
	OperandoILOC *destino_1_ponteiro = busca.eh_escopo_global ? reg_rbss() : reg_rfp();
	OperandoILOC *destino_2_deslocamento = gera_operando_imediato(busca.deslocamento);

	if(tem_buracos(expressao)) {
		rotulo_store = gera_nome_rotulo();
		CodigoILOC *codigo = atribui_booleano(expressao, rotulo_store);
		codigo_append_nodo(atribuicao, expressao);
		_append(atribuicao, codigo);
	} else {
		codigo_append_nodo(atribuicao, expressao);
	}
	origem = copia_operando(expressao->reg_resultado); //TODO CUIDAR!!!!!!!!!! TEM Q COPIAR SE FOR USAR EM NOVA INSTRUÇAO
	_cria_codigo_com_label_append(atribuicao, copia_nome(rotulo_store), origem, STOREAI, lista(destino_1_ponteiro, destino_2_deslocamento));
	
	atribuicao->reg_resultado = destino_1_ponteiro; //precisa linkar o resultado da atribuição com esses dois regs? Acho q n pq atribuição não é uma expressão. entao n deve ter reg resultado.
	//agr eu preciso kk

	print_ILOC_intermed("Codigo atribuicao", atribuicao->codigo);
}

void codigo_update_deslocamento(Nodo *nodo) {

	if(nodo == NULL) return;

	if(nodo->tipo_operacao == nodo_attr) {
		Nodo *identificador = nodo->filho;
		DeslocamentoEscopo busca = busca_deslocamento_e_escopo(identificador->valor_lexico.label);

		if(nodo->reg_resultado) {
			OperandoILOC *deslocamento = nodo->reg_resultado->proximo;
			if(deslocamento) deslocamento->valor = busca.deslocamento;
		}

		if(identificador->irmao) {
			Nodo *prox_attr = identificador->irmao->irmao;
			codigo_update_deslocamento(prox_attr);
		}
	}
}

//Ex.: L1: add r1, r2 => r3
void codigo_expr_aritmetica(Nodo *esq, Nodo *operador, Nodo *dir) {

    OperandoILOC *r1 = copia_operando(esq->reg_resultado);
	 OperandoILOC *r2 = copia_operando(dir->reg_resultado);
	 OperandoILOC *r3 = gera_operando_registrador(gera_nome_registrador());

	codigo_append_nodo(operador, esq);
	codigo_append_nodo(operador, dir);
	_cria_codigo_append(operador, lista(r1, r2), operacao_iloc_binaria_nodo(operador), r3);
    
	operador->reg_resultado = r3;

	print_ILOC_intermed("Codigo expr aritmetica", operador->codigo);
}

void codigo_expr_logica(Nodo *esq, Nodo *nodo_operador, Nodo *dir) {

	if(nodo_operador->tipo_operacao == nodo_and) {
		codigo_expr_logica_and(esq, nodo_operador, dir);
	}
	else if(nodo_operador->tipo_operacao == nodo_or)
		codigo_expr_logica_or(esq, nodo_operador, dir);
	else
		codigo_expr_logica_relacional(esq, nodo_operador, dir);
}

void codigo_expr_logica_relacional(Nodo *esq, Nodo *operador, Nodo *dir) {

	OperandoILOC *op_remendo_true = gera_operando_remendo();
	OperandoILOC *op_remendo_false = gera_operando_remendo();

   OperandoILOC *r1, *r2, *r3 = gera_operando_registrador(gera_nome_registrador());

	r1 = copia_operando(esq->reg_resultado);
   r2 = copia_operando(dir->reg_resultado);

	codigo_append_nodo(operador, esq);
	codigo_append_nodo(operador, dir);

	_cria_codigo_append(operador, lista(r1, r2), operacao_iloc_binaria_nodo(operador), r3);

	operador->reg_resultado = r3;

	CodigoILOC *codigo = codigo_compara_logico(copia_operando(r3), op_remendo_true, op_remendo_false);

	_append(operador, codigo);

	operador->remendos_true = append_remendo(operador->remendos_true, op_remendo_true);
	operador->remendos_false = append_remendo(operador->remendos_false, op_remendo_false);
}

void codigo_expr_logica_and(Nodo *esq, Nodo *operador, Nodo *dir) {

	char *rotulo = gera_nome_rotulo();
	char *rotulo_copia = copia_nome(rotulo);

	esq->remendos_true = remenda(esq->remendos_true, gera_operando_rotulo(rotulo));

	operador->remendos_true = dir->remendos_true;

	operador->remendos_false = concat_remendos(esq->remendos_false, dir->remendos_false);

	codigo_append_nodo(operador, esq);
	_append(operador, instrucao_nop(rotulo_copia));
   codigo_append_nodo(operador, dir);
}

void codigo_expr_logica_or(Nodo *esq, Nodo *operador, Nodo *dir) {

	char *rotulo = gera_nome_rotulo();
	char *rotulo_copia = copia_nome(rotulo);

	esq->remendos_false = remenda(esq->remendos_false, gera_operando_rotulo(rotulo));

	operador->remendos_false = dir->remendos_false;
	operador->remendos_true = concat_remendos(esq->remendos_true, dir->remendos_true);

	codigo_append_nodo(operador, esq);
	_append(operador, instrucao_nop(rotulo_copia));
   codigo_append_nodo(operador, dir);
}

void codigo_expr_logica_booleano(Nodo *nodo, int valor) {

	OperandoILOC *op_remendo = gera_operando_remendo();

   CodigoILOC *codigo = instrucao_jumpI(op_remendo);

	if(valor == TRUE) {
		nodo->remendos_true = append_remendo(nodo->remendos_true, op_remendo);
		nodo->remendos_false = NULL;
	} else {
		nodo->remendos_true = NULL;
		nodo->remendos_false = append_remendo(nodo->remendos_false, op_remendo);
	}

	_append(nodo, codigo);
}

void codigo_not(Nodo *operador, Nodo *expr) {

	operador->remendos_true = expr->remendos_false;
	operador->remendos_false =  expr->remendos_true;

	codigo_append_nodo(operador, expr);
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
		codigo_append_nodo(operador, expr);
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

	CodigoILOC *codigo = NULL;
	codigo = _append_codigo(codigo, codigo_cmp_NE_0);
	codigo = _append_codigo(codigo, codigo_cbr);

	return codigo;
}

// rsubI r1, 0 => r3 // r3 = 0 - r1
void codigo_sub(Nodo *operador, Nodo *expr) {

    //int nextInstructionLabel = this->getLabel();

    // resolveArithmetic(operador, expr, getRegister(), nextInstructionLabel); TODO curto ciruito, depende do BoolFLOW

	OperandoILOC *origem_1 = copia_operando(operador->reg_resultado);
	OperandoILOC *origem_2 = gera_operando_imediato(0);
	
	OperandoILOC *destino = gera_operando_registrador(gera_nome_registrador());

	_cria_codigo_append(operador, lista(origem_1, origem_2), RSUBI, destino);

   operador->reg_resultado = destino;
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
	OperandoILOC *r3 = gera_operando_registrador(gera_nome_registrador());

	CodigoILOC *codigo_cmp_NE = _cria_codigo(lista(r1, r2) , CMP_NE, r3);

	CodigoILOC *codigo = NULL;
	codigo = _append_codigo(codigo, codigo_0);
	codigo = _append_codigo(codigo, codigo_cmp_NE);

	return codigo;
}

CodigoILOC *instrucao_nop(char* label) {
   return _cria_codigo_com_label(label, NULL, NOP, NULL);
}

// loadI c1 => r2 // r2 = c1
CodigoILOC *instrucao_loadI(int valor, char *label) {

   OperandoILOC *origem = gera_operando_imediato(valor);

   OperandoILOC *destino = gera_operando_registrador(gera_nome_registrador());

   CodigoILOC *codigo = _cria_codigo_com_label(label, origem, LOADI, destino);

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