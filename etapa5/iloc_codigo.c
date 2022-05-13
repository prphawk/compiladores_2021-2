#include "iloc_codigo.h"

/*
INFOS:
- Código em uma só passagem, com remendos nas expressoes booleanas/de controle de fluxo.
- A tabela tabela de símbolos ajuda bastante na hora de controlar o valor de offset dos ponteiros que carregam/escrevem em variáveis.
Sua atualização de tamanho total de rsp (topo da pilha depois do tamanho total das variáveis locais da função) das pode ser maior ou menor
dependendo se vc tem guardado variáveis literais no escopo global ou junto com as variáveis locais no escopo atual.
- Tem que fazer deep copy de todo ponteiro (rótulo/operando/instrução) cujo conteúdo vc queira reutilizar em outra instrução. 
Pra evitar double free ao liberar memória.
- Como estamos construindo o código das folhas para raíz, ele está na ordem invertida de como é impresso. Por isso o codigo
de um nodo (CodigoILOC, que é uma linked list) tem o atributo "anterior" para indicar outro elemento da lista. 
Dá pra pensar nele como uma pilha, o ponteiro de código de cada nodo retorna a última instrução adicionada.
- Recomendado ver o vídeo de exemplos de código ILOC pra fazer código de função e chamada de função.

PREPARO CHAMAR UMA FUNÇÃO: (pilha, de baixo para cima)

rfp + 20 -> endereço do valor de retorno

rfp + 16 -> empilha parametro 2 (e desloca em +4 o endereço do valor de retorno, que é o ultimo)
rfp + 12 -> empilha parametro 1 (e desloca em +4 o endereço do valor de retorno, que é o ultimo)

rfp + 8 -> o rsp atual
rfp + 4 -> o rfp atual
rfp + 0 -> endereço de retorno
*/

#define FALSE 0
#define TRUE 1

const int MIN_OFFSET_PARAMS = 12; // 12 -> (rfp, 8) é o ultimo endereço q usamos ao chamar uma funcao, o rfp 12 tá livre pra empilhar argumentos
extern int print_ILOC_intermed_global;

char *rotulo_main_global = NULL;

Remendo *remendos_rotulo_funcao_global = NULL;

void codigo_finaliza(Nodo *arvore) {

	// append halt ------------------
	_append(arvore, instrucao_halt());

	// prepend codigo de inicialização ----------------------------------------------------------------------
	int num_instr_incompleto = conta_instrucoes(arvore->codigo);

	// inicializa rsp e rfp (opcional)
	CodigoILOC *codigo_lst = NULL;
	codigo_lst = _append_codigo(codigo_lst, instrucao_loadI_reg(1024, NULL, reg_rsp()));
	codigo_lst = _append_codigo(codigo_lst, instrucao_loadI_reg(1024, NULL, reg_rfp()));

	// inicializa rbss com o endereço imediatamente depois da instrução halt
	codigo_lst = _append_codigo(codigo_lst, instrucao_loadI_reg(num_instr_incompleto + 4, NULL, reg_rbss()));

	// pula pro rotulo equivalente a main() (o rotulo de cada funcao é reconhecido na declaracao)
	CodigoILOC *codigo_jump_main = instrucao_jumpI(gera_operando_rotulo(copia_nome(rotulo_main_global)));

	codigo_lst = _append_codigo(codigo_lst, codigo_jump_main);
	codigo_lst = _append_codigo(codigo_lst, arvore->codigo);

	arvore->codigo = codigo_lst;
}

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

// copia e append codigo de um nodo filho a nodo pai
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

void codigo_declaracao_funcao(Nodo *cabecalho, Nodo *corpo) {

	char *rotulo = gera_nome_rotulo();
	_append(cabecalho, instrucao_nop(rotulo));

	// rotulo Lx da funcao para fazer jumpI, guardamos na tabela de simbolos para a chamar a partir do nome da funcao
	insere_rotulo_funcao(cabecalho->valor_lexico.label, copia_nome(rotulo));

	if(remendos_rotulo_funcao_global) {
		OperandoILOC *operando = gera_operando_rotulo(copia_nome(rotulo));
		remendos_rotulo_funcao_global = remenda(remendos_rotulo_funcao_global, operando);
	}

	int eh_main = compare_eq_str(cabecalho->valor_lexico.label, "main");

	if(eh_main) rotulo_main_global = rotulo;

	codigo_rsp_e_rfp_declaracao_funcao(cabecalho, eh_main);

	codigo_carrega_parametros(cabecalho);

	codigo_append_nodo(cabecalho, corpo); //com o resultado de return viu

	codigo_retorna_funcao(cabecalho);
}

// inicialização de rsp e rfp em funcoes chamadas
void codigo_rsp_e_rfp_declaracao_funcao(Nodo *cabecalho, int eh_main) {

	if(!eh_main) { //TODO talvez n precise
		CodigoILOC *codigo_copia_rsp_para_rfp = _cria_codigo(reg_rsp(), I2I, reg_rfp());
		_append(cabecalho, codigo_copia_rsp_para_rfp);
	}

	int deslocamento_var_locais = busca_deslocamento_rsp(cabecalho->valor_lexico.label);

	int quantidade_params = busca_quantidade_parametros_funcao_atual();

	// se estamos numa funcao chamada, offset_params_atual_global é o valor 12 de return (rfp, 12) + o deslocamento do empilhamento de parametros
	int deslocamento_total = busca_offset_base_vars_locais_funcao_atual() + deslocamento_var_locais;
	CodigoILOC *codigo_atualiza_rsp = instrucao_addi(reg_rsp(), deslocamento_total, reg_rsp()); 

	_append(cabecalho, codigo_atualiza_rsp);
}

void codigo_carrega_parametros(Nodo *cabecalho) {

	ArgumentoFuncaoLst *args = busca_parametros_funcao(cabecalho->valor_lexico.label);

	if(args == NULL) return;

	int offset_quantidade_parametros = _conta_argumentos(args);

	int count = 0;

	while((count < offset_quantidade_parametros) && args != NULL) {

		OperandoILOC *r0 = gera_operando_registrador(gera_nome_registrador());

		_append(cabecalho, instrucao_loadai(reg_rfp(), (MIN_OFFSET_PARAMS + (4 * count)), r0));

		DeslocamentoEscopo busca = busca_deslocamento_e_escopo(args->nome);
		OperandoILOC *destino_ponteiro = busca.eh_escopo_global ? reg_rbss() : reg_rfp();

		_append(cabecalho, instrucao_storeai(copia_operando(r0), destino_ponteiro, busca.deslocamento + busca_offset_base_vars_locais_funcao_atual()));

		args = args->proximo;
		count++;
	}
}

// loadI 73 => r0
// storeAI r0 => rfp, offsetReturnValue
void codigo_return(Nodo *nodo, Nodo *expressao) {

	char *rotulo_store = NULL;
	OperandoILOC *origem;

	if(tem_buracos(expressao)) {
		rotulo_store = gera_nome_rotulo();
		CodigoILOC *codigo = atribui_booleano(expressao, rotulo_store, NULL);
		codigo_append_nodo(nodo, expressao);
		_append(nodo, codigo);
	} else {
		codigo_append_nodo(nodo, expressao);
	}
	origem = copia_operando(expressao->reg_resultado);
	
	int quantidade_params = busca_quantidade_parametros_funcao_atual();
	OperandoILOC *destino = lista(reg_rfp(), gera_operando_imediato((quantidade_params * 4) + 12));

	_cria_codigo_com_label_append(nodo, copia_nome(rotulo_store), origem, STOREAI, destino);
	
	nodo->reg_resultado = destino; //precisa linkar o resultado da atribuição com esses dois regs? Acho q n pq atribuição não é uma expressão. entao n deve ter reg resultado.
	//agr eu preciso kk

	codigo_retorna_funcao(nodo);

	print_ILOC_intermed("Codigo return", nodo->codigo);

	//instrucao_loadI_reg()
}

/*
oadAI rfp, 0 => r0 //obtém end. retorno
loadAI rfp, 4 => r1 //obtém rsp salvo
loadAI rfp, 8 => r2 //obtém rfp salvo
storeAI r1 => rsp
storeAI r2 => rfp
jump => r0
*/
void codigo_retorna_funcao(Nodo *cabecalho) {

	if(eh_a_main()) return; //TODO tirar se necessario

	OperandoILOC *r0 = gera_operando_registrador(gera_nome_registrador());
	OperandoILOC *r1 = gera_operando_registrador(gera_nome_registrador());
	OperandoILOC *r2 = gera_operando_registrador(gera_nome_registrador());

	_append(cabecalho, instrucao_loadai(reg_rfp(), 0, r0));
	_append(cabecalho, instrucao_loadai(reg_rfp(), 4, r1));
	_append(cabecalho, instrucao_loadai(reg_rfp(), 8, r2));

	_append(cabecalho, _cria_codigo(copia_operando(r1), I2I, reg_rsp()));
	_append(cabecalho, _cria_codigo(copia_operando(r2), I2I, reg_rfp()));

	_append(cabecalho, _cria_codigo(NULL, JUMP, copia_operando(r0)));
}

//loadAI originRegister, originOffset => resultRegister // r3 = Memoria(r1 + c2)
void codigo_carrega_variavel(Nodo *nodo) {

	int offset_params = busca_quantidade_parametros_funcao_atual();

   DeslocamentoEscopo busca = busca_deslocamento_e_escopo(nodo->valor_lexico.label);

   int deslocamento = busca.deslocamento;

   OperandoILOC *origem_1_registrador = busca.eh_escopo_global ? reg_rbss() : reg_rfp();
   OperandoILOC *origem_2_deslocamento = gera_operando_imediato(deslocamento + busca_offset_base_vars_locais_funcao_atual());

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

CodigoILOC *atribui_booleano(Nodo *expressao, char* rotulo_final, OperandoILOC *destino) {

	if(destino == NULL){
		destino = gera_operando_registrador(gera_nome_registrador());
	}

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

// taca buraco
void converte_para_logica(Nodo *expressao) {

	if(tem_buracos(expressao) || expressao->reg_resultado == NULL) {
		return;
	}

	OperandoILOC *op_remendo_true = gera_operando_remendo();
	OperandoILOC *op_remendo_false = gera_operando_remendo();

	OperandoILOC *reg_resultado = copia_operando(expressao->reg_resultado);

	CodigoILOC *codigo = codigo_compara_logico(reg_resultado, op_remendo_true, op_remendo_false);

	_append(expressao, codigo);

	expressao->remendos_true = append_remendo(expressao->remendos_true, op_remendo_true);
	expressao->remendos_false = append_remendo(expressao->remendos_false, op_remendo_false);

	print_ILOC_intermed("Codigo converte para logico", expressao->codigo);

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

	char *rotulo_fim = gera_nome_rotulo();
	char *rotulo_true = gera_nome_rotulo();
	char *rotulo_false = NULL;
	CodigoILOC *codigo_nop_false = NULL;
	CodigoILOC *codigo_jump_fim_copia = NULL;

	converte_para_logica(expressao);

	expressao->remendos_true = remenda(expressao->remendos_true, gera_operando_rotulo(copia_nome(rotulo_true)));

	CodigoILOC *codigo_nop_true = instrucao_nop(rotulo_true);
	CodigoILOC *codigo_jump_fim = instrucao_jumpI(gera_operando_rotulo(copia_nome(rotulo_fim)));

	if(bloco_false != NULL) {
		rotulo_false = gera_nome_rotulo();
		codigo_nop_false = instrucao_nop(copia_nome(rotulo_false));
		codigo_jump_fim_copia = copia_codigo(codigo_jump_fim);
	} else {
		rotulo_false = copia_nome(rotulo_fim);
	}

	expressao->remendos_false = remenda(expressao->remendos_false, gera_operando_rotulo(rotulo_false));

	CodigoILOC *codigo_nop_fim = instrucao_nop(rotulo_fim);

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

	OperandoILOC *r1 = gera_operando_registrador(gera_nome_registrador());

	// storeAI r1 => rsp, 0
	_append(nodo, instrucao_storeai(copia_operando(r1), reg_rsp(), 0));
	// storeAI rsp => rsp, 4
	_append(nodo, instrucao_storeai(reg_rsp(), reg_rsp(), 4));
	// storeAI rfp => rsp, 8
	_append(nodo, instrucao_storeai(reg_rfp(), reg_rsp(), 8));

	int offset_return = empilha_argumentos_chamada_funcao(nodo, lista_argumentos);

	//	loadI PC + 2 => r1 // guarda o endereço de retorno
	_append(nodo, instrucao_addi(reg_rpc(), 2, r1));

	// jumpI => L0 // pula pra funcao chamada
	char **rotulo_ptr = busca_rotulo_funcao(nome_funcao);
	OperandoILOC *operando = NULL;

	// caso esteja se chamando recursivamente e não tem label ainda
	if(rotulo_ptr && *rotulo_ptr == NULL) {
		operando = gera_operando_remendo();
		remendos_rotulo_funcao_global = append_remendo(remendos_rotulo_funcao_global, operando);
	} else {
		operando = gera_operando_rotulo(copia_nome(rotulo_ptr ? *rotulo_ptr : NULL));
	}
	
	_append(nodo, instrucao_jumpI(operando));

	//loadAI rsp, 12 => r0 // pega o retorno da funcao (12 se não tiver sido empilhado parametros)
	OperandoILOC *r0 = gera_operando_registrador(gera_nome_registrador());
	_append(nodo, instrucao_loadai(reg_rsp(), offset_return, r0));

	nodo->reg_resultado = r0;
}

//ex: storeAI r1 => r2, c3 // Memoria(r2 + c3) = r1
CodigoILOC *instrucao_storeai(OperandoILOC *r1, OperandoILOC *r2, int valor) {
	return _cria_codigo(r1, STOREAI, lista(r2, gera_operando_imediato(valor)));
}
//ex: r1, c2 => r3 // r3 = Memoria(r1 + c2)
CodigoILOC *instrucao_loadai(OperandoILOC *r1, int valor, OperandoILOC *r3) {
	return _cria_codigo(lista(r1, gera_operando_imediato(valor)), LOADAI, r3);
}

int empilha_argumentos_chamada_funcao(Nodo *chamada_funcao, Nodo *lista_argumentos) {
	Nodo *arg_lst = lista_argumentos;
	int count = 0;

	while(arg_lst != NULL) {

		if(tem_buracos(arg_lst)) {
			char *rotulo_fim_arg = gera_nome_rotulo();
			CodigoILOC *codigo = atribui_booleano(arg_lst, rotulo_fim_arg, NULL);
			codigo_append_nodo(chamada_funcao, arg_lst);
			_append(chamada_funcao, codigo);

			CodigoILOC *nop_fim_arg = instrucao_nop(copia_nome(rotulo_fim_arg));
			_append(chamada_funcao, nop_fim_arg);
		} else {
			codigo_append_nodo(chamada_funcao, arg_lst);
		}

		OperandoILOC *origem = copia_operando(arg_lst->reg_resultado);
		_append(chamada_funcao, instrucao_storeai(origem, reg_rsp(), MIN_OFFSET_PARAMS + (count * 4)));
		arg_lst = arg_lst->irmao;
		count++;
	}
	
	return MIN_OFFSET_PARAMS + (count * 4);
}


// storeAI r0 => r1 (rfp ou rbss), deslocamento
void codigo_atribuicao(Nodo *variavel, Nodo *atribuicao, Nodo *expressao) {
	
	int offset_params = busca_quantidade_parametros_funcao_atual();

	char *rotulo_store = NULL;
	OperandoILOC *origem;
	DeslocamentoEscopo busca = busca_deslocamento_e_escopo(variavel->valor_lexico.label);
	OperandoILOC *destino_1_ponteiro = busca.eh_escopo_global ? reg_rbss() : reg_rfp();
	OperandoILOC *destino_2_deslocamento = gera_operando_imediato(busca.deslocamento + busca_offset_base_vars_locais_funcao_atual());

	if(tem_buracos(expressao)) {
		rotulo_store = gera_nome_rotulo();
		CodigoILOC *codigo = atribui_booleano(expressao, rotulo_store, NULL);
		codigo_append_nodo(atribuicao, expressao);
		_append(atribuicao, codigo);
	} else {
		codigo_append_nodo(atribuicao, expressao);
	}
	origem = copia_operando(expressao->reg_resultado);
	_cria_codigo_com_label_append(atribuicao, copia_nome(rotulo_store), origem, STOREAI, lista(destino_1_ponteiro, destino_2_deslocamento));
	
	atribuicao->reg_resultado = destino_1_ponteiro;

	print_ILOC_intermed("Codigo atribuicao", atribuicao->codigo);
}

void codigo_update_deslocamento(Nodo *nodo) {

	if(nodo == NULL) return;

	if(nodo->tipo_operacao == nodo_attr_pend) {
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

	if(!tem_buracos(esq))
		converte_para_logica(esq);
	if(!tem_buracos(dir))
		converte_para_logica(dir);

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

	if(!tem_buracos(esq))
		converte_para_logica(esq);
	if(!tem_buracos(dir))
		converte_para_logica(dir);

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

	converte_para_logica(expr);

	operador->remendos_true = expr->remendos_false;
	operador->remendos_false =  expr->remendos_true;

	codigo_append_nodo(operador, expr);
}

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

	if(operador->tipo_operacao == nodo_sub || operador->tipo_operacao == nodo_neg) {
		//print_codigo(expr->codigo);
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

	if(operador->tipo_operacao == nodo_neg) {
		codigo_append_nodo(operador, expr);
	}

	OperandoILOC *origem_1 = copia_operando(expr->reg_resultado);
	OperandoILOC *origem_2 = gera_operando_imediato(0);
	
	OperandoILOC *destino = gera_operando_registrador(gera_nome_registrador());

	_cria_codigo_append(operador, lista(origem_1, origem_2), RSUBI, destino);

   operador->reg_resultado = destino;

	print_ILOC_intermed("codigo sub", operador->codigo);
}
   
//#endregion Código 

//#region Instrução 

//ex: addI r1, c2 => r3
CodigoILOC* instrucao_addi(OperandoILOC *r1, int valor, OperandoILOC *r3) {
	OperandoILOC *origem = lista(r1, gera_operando_imediato(valor));
	return _cria_codigo(origem, ADDI, r3);
}

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

CodigoILOC *instrucao_halt() {
	return _cria_codigo(NULL, HALT, NULL);
}

//#endregion Instrução 