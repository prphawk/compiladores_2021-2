#include "iloc_codigo.h"
#include "tabela_simbolos.h"

#define FALSE 0
#define TRUE 1

//#region Principais Código 

void cria_codigo_e_append(Nodo* nodo, OperandoCodigo *origem, Operacao operacao, OperandoCodigo *destino) 
{
	CodigoILOC *codigo = _cria_codigo(origem, operacao, destino);
    _append_codigo(nodo, codigo);
}

void _append_codigo(Nodo *nodo, CodigoILOC *codigo) 
{
	codigo->anterior = nodo->codigo;
   nodo->codigo = codigo;
}

void _append_codigo_de_nodo(Nodo *nodo, Nodo *nodo_append) {
   CodigoILOC *codigo_append = nodo_append->codigo;

   while(codigo_append->anterior != NULL) { //subo até o topo da instrução
      codigo_append = codigo_append->anterior;
   }

   codigo_append->anterior = nodo->codigo; //no topo, ligo o fim do codigo do nodo com o inicio do outro nodo
   nodo->codigo = codigo_append; //codigo inteiro no primeiro nodo
}

CodigoILOC *_cria_codigo(OperandoCodigo *origem, Operacao operacao, OperandoCodigo *destino)
{
    CodigoILOC *codigo = malloc(sizeof(CodigoILOC));
    codigo->label = NULL;
    codigo->origem = origem;
    codigo->operacao = operacao;
    codigo->destino = destino;

    return codigo;
}

CodigoILOC *_cria_codigo_com_label(char *label, OperandoCodigo *origem, Operacao operacao, OperandoCodigo *destino)
{
   CodigoILOC *codigo = _cria_codigo(origem, operacao, destino);
   codigo->label = label;
   return codigo;
}

void cria_codigo_com_label_e_append(Nodo* nodo, char *label, OperandoCodigo *origem, Operacao operacao, OperandoCodigo *destino)
{
   CodigoILOC *codigo = _cria_codigo_com_label(label, origem, operacao, destino);
   _append_codigo(nodo, codigo);
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
    OperandoCodigo *operando_label_false = (nodo_bloco_else == NULL) ? operando_fim : operando_bloco_else;
    
    CodigoILOC *jump_fim = instrucao_jump(operando_fim);
    
    //TODO resolveLogical(ifNode, expNode, operando_label_true, operando_label_false);

   _append_codigo(nodo_if, nop_com_label_bloco_if);
   _append_codigo_de_nodo(nodo_if, nodo_bloco_if);
   
   if (nodo_bloco_else != NULL) {
      
      _append_codigo(nodo_if, {jump_fim});
      _append_codigo(nodo_if, {nop_com_label_bloco_else});
      _append_codigo_de_nodo(nodo_if, nodo_bloco_else);
      
   }
   
   _append_codigo(ifNode, {nop_com_label_fim});
}

CodigoILOC *instrucao_nop(char* label) {
   return _cria_codigo_com_label(label, NULL, NOP, NULL);
} 

//loadAI originRegister, originOffset => resultRegister // r3 = Memoria(r1 + c2)
void codigo_carrega_variavel(Nodo *nodo) {

   DeslocamentoEscopo busca = busca_deslocamento_e_escopo(nodo->valor_lexico.label);

   int deslocamento = busca.deslocamento;

   OperandoCodigo *origem_1_registrador = busca.eh_escopo_global ? cria_rbss() : cria_rfp();
   OperandoCodigo *origem_2_deslocamento = cria_operando_imediato(deslocamento);

   origem_1_registrador->proximo = origem_2_deslocamento;

   OperandoCodigo *destino = cria_operando_registrador(gera_nome_registrador());

   cria_codigo_e_append(nodo, origem_1_registrador, LOADAI, destino);

   //TODO e o codigo de nodo? n tem na referencia nodo->codigo = global_codigo;
   nodo->resultado = destino;
}

// loadI c1 => r2 // r2 = c1
void codigo_carrega_literal(Nodo *nodo) {

   int valor = nodo->valor_lexico.valor_int; //2.3: Simplificações para a Geração de Código

   CodigoILOC *codigo = instrucao_loadI(valor);

   _append_codigo(nodo, codigo);
   
   nodo->codigo = codigo;
   nodo->resultado = codigo->destino;
}

// storeAI r0 => r1 (rfp ou rbss), deslocamento
void codigo_atribuicao(Nodo *variavel, Nodo *atribuicao, Nodo *expressao) {

    //if (variavel->tipo != TIPO_INT) return;

	OperandoCodigo *origem = NULL;

	DeslocamentoEscopo busca = busca_deslocamento_e_escopo(variavel->valor_lexico.label);
	
	OperandoCodigo *destino_1_ponteiro = busca.eh_escopo_global ? cria_rbss() : cria_rfp();
	OperandoCodigo *destino_2_deslocamento = cria_operando_imediato(busca.deslocamento);

	//if(expressao->com_curto_circuito) {
		OperandoCodigo *origem = expressao->resultado; 
	//} else { //TODO botar curto circuito de expressoes!

	}

	liga_operandos(destino_1_ponteiro, destino_2_deslocamento);

	cria_codigo_e_append(atribuicao, origem, STOREAI, destino_1_ponteiro);
	
	//variavel->codigo = global_codigo;
	//nodo->resultado = destino;
}

/* 
L2:loadI true => r5
   jumpI -> L5
L3:loadI false => r5
   jumpI -> L5
*/
void codigo_logico(Nodo *nodo)
{
   char *registrador_result = gera_nome_registrador();
   char *label_true = gera_nome_rotulo();
   char *label_false = gera_nome_rotulo();
   char *label_fim = gera_nome_rotulo();

   codigo_logico_auxiliar(NULL, nodo, label_true, label_fim);
   
   OperandoCodigo *origem_load_true = cria_operando_imediato(1);

   OperandoCodigo *destino_load_true = cria_operando_registrador(registrador_result);

   cria_codigo_com_label_e_append(nodo, label_true, origem_load_true, LOADI, destino_load_true);

   OperandoCodigo *destino_jump_true = cria_operando_label(label_fim);

   cria_codigo_e_append(nodo, NULL, JUMPI, destino_jump_true);
    
   OperandoCodigo *origem_load_false = cria_operando_imediato(0);

   OperandoCodigo *destino_load_false = cria_operando_registrador(registrador_result);

   cria_codigo_com_label_e_append(nodo, label_false, origem_load_false, LOADI, destino_load_false);

   OperandoCodigo *destino_jump_false = cria_operando_label(label_fim);

   cria_codigo_e_append(nodo, NULL, JUMPI, destino_jump_false);
   cria_codigo_com_label_e_append(nodo, label_fim, NULL, NOP, NULL); //TODO tirar essa gambiarra?
   //nodo->codigo = global_codigo;
}

void codigo_expr_unaria(Nodo *nodo_operacao, Nodo *nodo) {

	if(nodo_operacao->operacao == SUB) codigo_sub(nodo_operacao, nodo);
	else {
        //appendCode(operacao, nodo); TODO
		nodo_operacao->resultado = nodo->resultado;
    }
}

// rsubI r1, 0 => r3 // r3 = 0 - r1
void codigo_sub(Nodo *nodo_operacao, Nodo *nodo) {

    //int nextInstructionLabel = this->getLabel();

    // resolveArithmetic(symbolNode, expressionNode, getRegister(), nextInstructionLabel); TODO curto ciruito, depende do BoolFLOW

	OperandoCodigo *origem_1 = nodo_operacao->resultado;
	OperandoCodigo *origem_2 = cria_operando_imediato(0);
	liga_operandos(origem_1, origem_2);

	OperandoCodigo *destino = cria_operando_registrador(gera_nome_registrador());

	cria_codigo_e_append(nodo_operacao, origem_1, RSUBI, destino);

    nodo_operacao->resultado = destino;
}


void codigo_logico_auxiliar(char *label, Nodo *nodo, char* label_true, char* label_false) {
   char *label1 = gera_nome_rotulo();
   switch(nodo->operacao)
   {
      /* [A<B]
         cmp_LT ra, rb => cc1
         cbr cc1 -> L1, L2
      L1:loadI true => r1
         jumpI -> L3
      L2:loadI false => r1
         jumpI -> L3
      */
      case CMP_EQ:
         codigo_logico_operacoes(nodo, label, CMP_EQ, label_true, label_false);
      break;

      case CMP_NE:
         codigo_logico_operacoes(nodo, label, CMP_NE, label_true, label_false);
      break;

      case CMP_LE:
         codigo_logico_operacoes(nodo, label, CMP_LE, label_true, label_false);
      break;

      case CMP_GE:
         codigo_logico_operacoes(nodo, label, CMP_GE, label_true, label_false);
      break;

      case CMP_GT:
         codigo_logico_operacoes(nodo, label, CMP_GT, label_true, label_false);
      break;

      case CMP_LT:
         codigo_logico_operacoes(nodo, label, CMP_LT, label_true, label_false);
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
      case OR:
         codigo_logico_auxiliar(label, nodo->filho, label_true, label1);
         codigo_logico_auxiliar(label1, nodo->filho->irmao, label_true, label_false);
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
      case AND:
         codigo_logico_auxiliar(label, nodo->filho, label1, label_false);
         codigo_logico_auxiliar(label1, nodo->filho->irmao, label_true, label_false);
      break;
      
      default:
      break;
   }
}

void codigo_logico_operacoes(Nodo *nodo, char *label, Operacao operacao, char* label_true, char* label_false)
{
   char *registradorA = gera_nome_registrador();
   char *registradorB = gera_nome_registrador();
   char *registradorCC1 = gera_nome_registrador();

   OperandoCodigo *op_origem = cria_operando_registrador(registradorA);
   OperandoCodigo *op_origem2 = cria_operando_registrador(registradorB);
   liga_operandos(op_origem,op_origem2);
   OperandoCodigo *op_destino = cria_operando_registrador(registradorCC1);

   cria_codigo_com_label_e_append(nodo, label, op_origem, operacao, op_destino);

   OperandoCodigo *cbr_origem = cria_operando_registrador(registradorCC1);
   OperandoCodigo *cbr_destino = cria_operando_registrador(label_true);
   OperandoCodigo *cbr_destino2 = cria_operando_registrador(label_false);
   liga_operandos(cbr_destino,cbr_destino2);

   cria_codigo_e_append(nodo, cbr_origem, CMP_EQ, cbr_destino);
}
//#endregion Código 

//#region Instrução 

// loadI c1 => r2 // r2 = c1
CodigoILOC *instrucao_loadI(int valor) {

   OperandoCodigo *origem = cria_operando_imediato(valor);

   OperandoCodigo *destino = cria_operando_registrador(gera_nome_registrador());

   CodigoILOC *codigo = _cria_codigo(origem, LOADI, destino);

   return codigo;
}


// ex: jumpI -> l1 // PC = endereço(l1)
CodigoILOC *instrucao_jump(OperandoCodigo* destino) {

   return _cria_codigo(NULL, JUMPI, destino);
}

//#endregion Instrução 