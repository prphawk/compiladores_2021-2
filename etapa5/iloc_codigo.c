#include "iloc_codigo.h"

#define FALSE 0
#define TRUE 1

int print_stuff = 1;

//#region Principais Código 

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

void _append_nodo(Nodo *pai, Nodo *filho) {

   CodigoILOC *filho_cod_ptr = filho->codigo;

   CodigoILOC *copia_filho_cod_ptr = copia_codigo(filho_cod_ptr);

   _append(pai, copia_filho_cod_ptr);
}

//#endregion Principais Código 

//#region Código 

//exp code
//x:nop
//ifblocknode code
//jump z
//y:nop
//elseBlockNode code
//z:nop

//ou

//exp code
//x:nop
//ifblocknode code
//z:nop
/*
void codigo_if(Nodo *nodo_if, Nodo *nodo_expr, Nodo *nodo_bloco_if, Nodo *nodo_bloco_else) {

//TODO mudar nomes e retirar comentario acima
    char* label_bloco_if = gera_nome_rotulo();
    char* label_bloco_else = gera_nome_rotulo();
    char* label_fim = gera_nome_rotulo();
    
    OperandoCodigo *operando_bloco_if = cria_operando_label(label_bloco_if);
    OperandoCodigo *operando_bloco_else = cria_operando_label(label_bloco_else);
    OperandoCodigo *operando_fim = cria_operando_label(label_fim);

    CodigoILOC *nop_com_label_bloco_if = instrucao_nop(label_bloco_if);
    CodigoILOC *nop_com_label_bloco_else = instrucao_nop(label_bloco_else);
    CodigoILOC *nop_com_label_fim = instrucao_nop(label_fim);

    //where it jumps in case expNode is true
    OperandoCodigo *operando_label_true = operando_bloco_if;

    //where it jumps in case expNode is false
    OperandoCodigo *operando_label_false = nodo_bloco_else == NULL ? operando_fim : operando_bloco_else;
    
    CodigoILOC *jump_fim = instrucao_jump(operando_fim);
    
    //TODO resolveLogical(ifNode, expNode, operando_label_true, operando_label_false);

   //      if (nodeExp->hasPatchworks) {

   //      coverPatchworks(nodeExp, labelTrue, true);
   //      coverPatchworks(nodeExp, labelFalse, false);
   //      appendCode(rootNode, nodeExp);
        
   //  } else {

        OperandoCodigo r1Operand = nodeExp->resultRegister;
        list<InstructionCode> compareCode = makeCompare(r1Operand, labelTrue, labelFalse);
        appendCode(rootNode, nodeExp);
        appendCode(rootNode, compareCode);
    //}

   _append_codigo(nodo_if, nop_com_label_bloco_if);
   _append_codigo_de_nodo(nodo_if, nodo_bloco_if);
   
   if (nodo_bloco_else != NULL) {
      
      _append_codigo(nodo_if, {jump_fim});
      _append_codigo(nodo_if, {nop_com_label_bloco_else});
      _append_codigo_de_nodo(nodo_if, nodo_bloco_else);
      
   }

   _append_codigo(ifNode, {nop_com_label_fim});
}
*/

// // cbr r1 -> l2_true, l3_false // PC = endereço(l2) se r1 = true, senão PC = endereço(l3)
// CodigoILOC* codigo_verifica_expr(OperandoCodigo *operando_registrador, OperandoCodigo *operando_label_true, OperandoCodigo *operando_label_false) {

//    liga_operandos(operando_label_true, operando_label_false);
//    return _cria_codigo(operando_registrador, CBR, operando_label_true);
// }


//loadAI originRegister, originOffset => resultRegister // r3 = Memoria(r1 + c2)
void codigo_carrega_variavel(Nodo *nodo) {

   DeslocamentoEscopo busca = busca_deslocamento_e_escopo(nodo->valor_lexico.label);

   int deslocamento = busca.deslocamento;

   OperandoILOC *origem_1_registrador = busca.eh_escopo_global ? reg_rbss() : reg_rfp();
   OperandoILOC *origem_2_deslocamento = operando_imediato(deslocamento);

   OperandoILOC *destino = operando_registrador(gera_nome_registrador());

   _cria_codigo_append(nodo, lista(origem_1_registrador, origem_2_deslocamento), LOADAI, destino);

   nodo->reg_resultado = destino;

	if(print_stuff) {
		printf("\n>> OP: carrega variavel\n");
		imprime_codigo(nodo->codigo);
   }
}

// loadI c1 => r2 // r2 = c1
void codigo_carrega_literal(Nodo *nodo) {

   if(!tem_valor_literal_int(nodo->valor_lexico)) return;

   CodigoILOC *codigo = instrucao_loadI(nodo->valor_lexico.valor_int);

   _append(nodo, codigo);
   
   nodo->reg_resultado = codigo->destino;

   if(print_stuff) {
		printf("\n>> OP: carrega literal\n");
		imprime_codigo(codigo);
   }
}

// loadI c1 => r2 // r2 = c1
void codigo_carrega_booleano(Nodo *nodo, int valor) {

   //int valor = nodo->valor_lexico.valor_int; //2.3: Simplificações para a Geração de Código

   CodigoILOC *codigo = instrucao_loadI(valor);

   _append(nodo, codigo);
   
   nodo->reg_resultado = codigo->destino;
}

// storeAI r0 => r1 (rfp ou rbss), deslocamento // TODO: n passar o load da variavel
void codigo_atribuicao(Nodo *variavel, Nodo *atribuicao, Nodo *expressao) {

	DeslocamentoEscopo busca = busca_deslocamento_e_escopo(variavel->valor_lexico.label);
	
	OperandoILOC *destino_1_ponteiro = busca.eh_escopo_global ? reg_rbss() : reg_rfp();
	OperandoILOC *destino_2_deslocamento = operando_imediato(busca.deslocamento);

   _append_nodo(atribuicao, expressao);

	//if(expressao->com_curto_circuito) {
		OperandoILOC *origem = copia_operando(expressao->reg_resultado); //TODO CUIDAR!!!!!!!!!! TEM Q COPIAR

	//} else { //TODO botar curto circuito de expressoes!

	//}
	_cria_codigo_append(atribuicao, origem, STOREAI, lista(destino_1_ponteiro, destino_2_deslocamento));
	
	//atribuicao->reg_resultado = destino_1_ponteiro; //precisa linkar o resultado da atribuição com esses dois regs?

   //libera_codigo(variavel->codigo); //TODO fazer com q libere os nodos filhos tbm

   if(print_stuff) {
      printf("\n>> OP: codigo atribuicao\n");
      imprime_codigo(atribuicao->codigo);
   }
}

//Ex.: L1: add r1, r2 => r3
void codigo_expr_aritmetica(Nodo *esq, Nodo *operador, Nodo *dir) {

    char* operacao_label;
    OperandoILOC *r3 = operando_registrador(gera_nome_registrador());
    OperandoILOC *r1;
    OperandoILOC *r2;

    if (!esq->tem_remendo && !dir->tem_remendo) 
         operacao_label = NULL;
    else operacao_label = gera_nome_rotulo();

    codigo_operador_expr(operador, esq, gera_nome_registrador(), operacao_label);
    r1 = operador->reg_resultado;

    codigo_operador_expr(operador, dir, gera_nome_registrador(), operacao_label);
    r2 = operador->reg_resultado;

    _cria_codigo_com_label_append(operador, operacao_label, lista(r1, r2), operacao_iloc_binaria_nodo(operador), r3);
    
    operador->reg_resultado = r3;
    operador->tem_remendo = FALSE;

   if(print_stuff) {
      printf("\n>> OP: codigo expr aritmetica\n");
      imprime_codigo(operador->codigo);
   }
}

//dar um jeito nesse monte de repeticao de operadores no codigo inteiro.. 
int operacao_iloc_binaria_nodo(Nodo *nodo_operador) {
   switch(nodo_operador->operador) {
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

//TODO lidar com remendos
void codigo_operador_expr(Nodo *operador, Nodo *expr, char* reg_resultado_nome, char* operacao_label) {

    OperandoILOC *reg_resultado;

   //  if (expr->tem_remendo) {
        
   //      reg_resultado = operando_registrador(reg_resultado_nome);
   //      list<InstructionCode> newCode = createBoolFlow(nodeExp, operacao_label, resultRegisterOperand);
   //      _append_nodo(operador, expr);
   //      _append(operador, {newCode});
    
   //  } else {
        reg_resultado = expr->reg_resultado;
        _append_nodo(operador, expr);
    //}

   //libera_operando(operador->reg_resultado);
   operador->reg_resultado = reg_resultado;
   operador->tem_remendo = FALSE;
}

/* 
L2:loadI true => r5
   jumpI -> L5
L3:loadI false => r5
   jumpI -> L5
*/
void codigo_logico(Nodo *operador)
{
   char *registrador_result = gera_nome_registrador();
   char *label_true = gera_nome_rotulo();
   char *label_false = gera_nome_rotulo();
   char *label_fim = gera_nome_rotulo();

   codigo_logico_auxiliar(NULL, operador, label_true, label_fim);
   
   OperandoILOC *origem_load_true = operando_imediato(1);

   OperandoILOC *destino_load_true = operando_registrador(registrador_result);

   _cria_codigo_com_label_append(operador, label_true, origem_load_true, LOADI, destino_load_true);

   OperandoILOC *destino_jump_true = operando_label(label_fim);

   _cria_codigo_append(operador, NULL, JUMPI, destino_jump_true);
    
   OperandoILOC *origem_load_false = operando_imediato(0);

   OperandoILOC *destino_load_false = operando_registrador(registrador_result);

   _cria_codigo_com_label_append(operador, label_false, origem_load_false, LOADI, destino_load_false);

   OperandoILOC *destino_jump_false = operando_label(label_fim);

   _cria_codigo_append(operador, NULL, JUMPI, destino_jump_false);
   _cria_codigo_com_label_append(operador, label_fim, NULL, NOP, NULL); //TODO tirar essa gambiarra? botar o remendo

   operador->reg_resultado = operando_registrador(registrador_result);
   operador->tem_remendo = TRUE;
}

void codigo_expr_unaria(Nodo *operador, Nodo *expr) {

	if(operador->operador == nodo_sub) {
      codigo_sub(operador, expr);
   }
   else if(operador->operador == nodo_not) {
      codigo_not(operador, expr);
   }
	else {
      _append_nodo(operador, expr);
		operador->reg_resultado = expr->reg_resultado;
    }
}

// rsubI r1, 0 => r3 // r3 = 0 - r1
void codigo_sub(Nodo *operador, Nodo *expr) {

    //int nextInstructionLabel = this->getLabel();

    // resolveArithmetic(operador, expr, getRegister(), nextInstructionLabel); TODO curto ciruito, depende do BoolFLOW

	OperandoILOC *origem_1 = operador->reg_resultado;
	OperandoILOC *origem_2 = operando_imediato(0);
	
	OperandoILOC *destino = operando_registrador(gera_nome_registrador());

	_cria_codigo_append(operador, lista(origem_1, origem_2), RSUBI, destino);

   operador->reg_resultado = destino;
}

// TODO precisa resolver como lógica e trocar a ordem dos labels de true e false
void codigo_not(Nodo *operador, Nodo *expr) {

	 OperandoILOC *remendo_true = cria_operando_remendo_true();
    OperandoILOC *remendo_false = cria_operando_remendo_false();
    
    //make compare with inverted labels 
    //resolveLogical(operador, expr, remendo_false, remendo_true);
    
    operador->tem_remendo = TRUE;
}


void codigo_logico_auxiliar(char *label, Nodo *nodo_operador, char* label_true, char* label_false) {
   char *label1 = gera_nome_rotulo();
   switch(nodo_operador->operador)
   {
      /* [A<B]
         cmp_LT ra, rb => cc1
         cbr cc1 -> L1, L2
      L1:loadI true => r1
         jumpI -> L3
      L2:loadI false => r1
         jumpI -> L3
      */
      case nodo_EQ: //TODO ajeitar os tipos disso aqui
         codigo_logico_operacoes(nodo_operador, label, CMP_EQ, label_true, label_false);
      break;

      case nodo_NE:
         codigo_logico_operacoes(nodo_operador, label, CMP_NE, label_true, label_false);
      break;

      case nodo_LE:
         codigo_logico_operacoes(nodo_operador, label, CMP_LE, label_true, label_false);
      break;

      case nodo_GE:
         codigo_logico_operacoes(nodo_operador, label, CMP_GE, label_true, label_false);
      break;

      case nodo_GT:
         codigo_logico_operacoes(nodo_operador, label, CMP_GT, label_true, label_false);
      break;

      case nodo_LT:
         codigo_logico_operacoes(nodo_operador, label, CMP_LT, label_true, label_false);
      break;
      
      /* [A<B||C>B]
         cmp_LT ra, rb => cc1 //A<B
         cbr cc1 -> LT, L1
      L1:cmp_GT rc, rd => cc2 //C>D
         cbr cc2 -> LT, LF
      LT:loadI true => r5
         jumpI -> L5
      LF:loadI false => r5
         jumpI -> L5
      */
      case nodo_or:
         codigo_logico_auxiliar(label, nodo_operador->filho, label_true, label1);
         codigo_logico_auxiliar(label1, nodo_operador->filho->irmao, label_true, label_false);
      break;

      /* [A<B&&C>B]
         {cmp_LT ra, rb => cc1 //A<B
         cbr cc1 -> L1, LF}
      L1:{cmp_GT rc, rd => cc2 //C>D
         cbr cc2 -> LT, LF}
      LT:loadI true => r5
         jumpI -> L5
      LF:loadI false => r5
         jumpI -> L5
      */
     case nodo_and:
         codigo_logico_auxiliar(label, nodo_operador->filho, label1, label_false);
         codigo_logico_auxiliar(label1, nodo_operador->filho->irmao, label_true, label_false);
      break;
      
      default:
      break;
   }
}

void codigo_logico_operacoes(Nodo *operador, char *label, OperacaoILOC operacao, char* label_true, char* label_false)
{
   char *registradorA = gera_nome_registrador();
   char *registradorB = gera_nome_registrador();
   char *registradorCC1 = gera_nome_registrador();

   OperandoILOC *op_origem = operando_registrador(registradorA);
   OperandoILOC *op_origem2 = operando_registrador(registradorB);
   OperandoILOC *op_destino = operando_registrador(registradorCC1);

   _cria_codigo_com_label_append(operador, label, lista(op_origem,op_origem2), operacao, op_destino);

   OperandoILOC *cbr_origem = operando_registrador(registradorCC1);
   OperandoILOC *cbr_destino = operando_registrador(label_true);
   OperandoILOC *cbr_destino2 = operando_registrador(label_false);

   _cria_codigo_append(operador, cbr_origem, CMP_EQ, lista(cbr_destino,cbr_destino2));
}
//#endregion Código 

//#region Instrução 

CodigoILOC *instrucao_nop(char* label) {
   return _cria_codigo_com_label(label, NULL, NOP, NULL);
}

// loadI c1 => r2 // r2 = c1
CodigoILOC *instrucao_loadI(int valor) {

   OperandoILOC *origem = operando_imediato(valor);

   OperandoILOC *destino = operando_registrador(gera_nome_registrador());

   CodigoILOC *codigo = _cria_codigo(origem, LOADI, destino);

   return codigo;
}

// ex: jumpI -> l1 // PC = endereço(l1)
CodigoILOC *instrucao_jump(OperandoILOC* destino) {

   return _cria_codigo(NULL, JUMPI, destino);
}

//#endregion Instrução 