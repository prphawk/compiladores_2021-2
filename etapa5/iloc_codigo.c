#include "iloc_codigo.h"
#include "tabela_simbolos.h"

#define FALSE 0
#define TRUE 1

//#region Principais Código 

void cria_codigo_e_append(Nodo *nodo, OperandoILOC *origem, Operacao operacao, OperandoILOC *destino) 
{
	CodigoILOC *codigo = _cria_codigo(origem, operacao, destino);
    _append_codigo(nodo, codigo);
}

void _append_codigo(Nodo *nodo, CodigoILOC *codigo_fim_ptr)
{
   CodigoILOC *aux = codigo_fim_ptr; //fim_Codigo é o ponteiro que aponta para o FINAL de lista de codigo 
   while(aux->anterior != NULL) { //subo até o topo da instrução
      aux = aux->anterior; 
   } //chega no inicio do codigo a ser adicionado
   aux->anterior = nodo->codigo; //no topo, ligo o fim do codigo do nodo com o inicio do outro nodo
   nodo->codigo = codigo_fim_ptr; //codigo inteiro no primeiro nodo
}

CodigoILOC *_cria_codigo(OperandoILOC *origem, Operacao operacao, OperandoILOC *destino)
{
    CodigoILOC *codigo = malloc(sizeof(CodigoILOC));
    codigo->label = NULL;
    codigo->origem = origem;
    codigo->operacao = operacao;
    codigo->destino = destino;

    return codigo;
}

CodigoILOC *_cria_codigo_com_label(char *label, OperandoILOC *origem, Operacao operacao, OperandoILOC *destino)
{
   CodigoILOC *codigo = _cria_codigo(origem, operacao, destino);
   codigo->label = label;
   return codigo;
}

void cria_codigo_com_label_e_append(Nodo *nodo, char *label, OperandoILOC *origem, Operacao operacao, OperandoILOC *destino)
{
   CodigoILOC *codigo = _cria_codigo_com_label(label, origem, operacao, destino);
   _append_codigo(nodo, codigo);
}

void imprime_codigo_final(Nodo *arvore)
{
   imprime_codigo(arvore->codigo);
}

void imprime_codigo(CodigoILOC *codigo)
{
   if(codigo!=NULL)
   {
      imprime_codigo(codigo->anterior);
      
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

      imprime_operando(codigo->origem);

      printf(" => ");

      imprime_operando(codigo->destino);

      printf("\n");

   }
}

void imprime_operando(OperandoILOC *operando)
{
   if(operando!=NULL)
   {
      printf("%s", operando->nome);
      if(operando->proximo!=NULL)
         printf(", %s", operando->proximo->nome);
   }
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


CodigoILOC *instrucao_nop(char* label) {
   return _cria_codigo_com_label(label, NULL, NOP, NULL);
} 

//loadAI originRegister, originOffset => resultRegister // r3 = Memoria(r1 + c2)
void codigo_carrega_variavel(Nodo *nodo) {

   DeslocamentoEscopo busca = busca_deslocamento_e_escopo(nodo->valor_lexico.label);

   int deslocamento = busca.deslocamento;

   OperandoILOC *origem_1_registrador = busca.eh_escopo_global ? cria_rbss() : cria_rfp();
   OperandoILOC *origem_2_deslocamento = cria_operando_imediato(deslocamento);

   origem_1_registrador->proximo = origem_2_deslocamento;

   OperandoILOC *destino = cria_operando_registrador(gera_nome_registrador());

   cria_codigo_e_append(nodo, origem_1_registrador, LOADAI, destino);

   //TODO e o codigo de nodo? n tem na referencia nodo->codigo = global_codigo;
   nodo->reg_resultado = destino;
}

// loadI c1 => r2 // r2 = c1
void codigo_carrega_literal(Nodo *nodo, int valor) {

   //int valor = nodo->valor_lexico.valor_int; //2.3: Simplificações para a Geração de Código

   CodigoILOC *codigo = instrucao_loadI(valor);

   _append_codigo(nodo, codigo);
   
   nodo->reg_resultado = codigo->destino;
}

// storeAI r0 => r1 (rfp ou rbss), deslocamento TODO n passar o load
void codigo_atribuicao(Nodo *variavel, Nodo *atribuicao, Nodo *expressao) {

    //if (variavel->tipo != TIPO_INT) return;

	//OperandoCodigo *origem = NULL;

	DeslocamentoEscopo busca = busca_deslocamento_e_escopo(variavel->valor_lexico.label);
	
	OperandoILOC *destino_1_ponteiro = busca.eh_escopo_global ? cria_rbss() : cria_rfp();
	OperandoILOC *destino_2_deslocamento = cria_operando_imediato(busca.deslocamento);

	//if(expressao->com_curto_circuito) {
		OperandoILOC *origem = expressao->reg_resultado; 
	//} else { //TODO botar curto circuito de expressoes!

	//}

	cria_codigo_e_append(atribuicao, origem, STOREAI, lista(destino_1_ponteiro, destino_2_deslocamento));
	
	//variavel->codigo = global_codigo;
	//nodo->resultado = destino;
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
   
   OperandoILOC *origem_load_true = cria_operando_imediato(1);

   OperandoILOC *destino_load_true = cria_operando_registrador(registrador_result);

   cria_codigo_com_label_e_append(operador, label_true, origem_load_true, LOADI, destino_load_true);

   OperandoILOC *destino_jump_true = cria_operando_label(label_fim);

   cria_codigo_e_append(operador, NULL, JUMPI, destino_jump_true);
    
   OperandoILOC *origem_load_false = cria_operando_imediato(0);

   OperandoILOC *destino_load_false = cria_operando_registrador(registrador_result);

   cria_codigo_com_label_e_append(operador, label_false, origem_load_false, LOADI, destino_load_false);

   OperandoILOC *destino_jump_false = cria_operando_label(label_fim);

   cria_codigo_e_append(operador, NULL, JUMPI, destino_jump_false);
   cria_codigo_com_label_e_append(operador, label_fim, NULL, NOP, NULL); //TODO tirar essa gambiarra?
   //nodo->codigo = global_codigo;

   operador->reg_resultado = cria_operando_registrador(registrador_result);
}

void codigo_expr_unaria(Nodo *nodo_operacao, Nodo *nodo) {

	if(nodo_operacao->operacao == SUB) codigo_sub(nodo_operacao, nodo);
	else {
        //appendCode(operacao, nodo); TODO
		nodo_operacao->reg_resultado = nodo->reg_resultado;
    }
}

// rsubI r1, 0 => r3 // r3 = 0 - r1
void codigo_sub(Nodo *operador, Nodo *nodo) {

    //int nextInstructionLabel = this->getLabel();

    // resolveArithmetic(symbolNode, expressionNode, getRegister(), nextInstructionLabel); TODO curto ciruito, depende do BoolFLOW

	OperandoILOC *origem_1 = operador->reg_resultado;
	OperandoILOC *origem_2 = cria_operando_imediato(0);
	
	OperandoILOC *destino = cria_operando_registrador(gera_nome_registrador());

	cria_codigo_e_append(operador, lista(origem_1, origem_2), RSUBI, destino);

   operador->reg_resultado = destino;
}


void codigo_logico_auxiliar(char *label, Nodo *operador, char* label_true, char* label_false) {
   char *label1 = gera_nome_rotulo();
   switch(operador->operacao)
   {
      /* [A<B]
         cmp_LT ra, rb => cc1
         cbr cc1 -> L1, L2
      L1:loadI true => r1
         jumpI -> L3
      L2:loadI false => r1
         jumpI -> L3
      */
      case CMP_EQ: //TODO ajeitar os tipos disso aqui
         codigo_logico_operacoes(operador, label, CMP_EQ, label_true, label_false);
      break;

      case CMP_NE:
         codigo_logico_operacoes(operador, label, CMP_NE, label_true, label_false);
      break;

      case CMP_LE:
         codigo_logico_operacoes(operador, label, CMP_LE, label_true, label_false);
      break;

      case CMP_GE:
         codigo_logico_operacoes(operador, label, CMP_GE, label_true, label_false);
      break;

      case CMP_GT:
         codigo_logico_operacoes(operador, label, CMP_GT, label_true, label_false);
      break;

      case CMP_LT:
         codigo_logico_operacoes(operador, label, CMP_LT, label_true, label_false);
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
         codigo_logico_auxiliar(label, operador->filho, label_true, label1);
         codigo_logico_auxiliar(label1, operador->filho->irmao, label_true, label_false);
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

      break;
      
      default:
      break;
   }
}

void codigo_logico_operacoes(Nodo *operador, char *label, Operacao operacao, char* label_true, char* label_false)
{
   char *registradorA = gera_nome_registrador();
   char *registradorB = gera_nome_registrador();
   char *registradorCC1 = gera_nome_registrador();

   OperandoILOC *op_origem = cria_operando_registrador(registradorA);
   OperandoILOC *op_origem2 = cria_operando_registrador(registradorB);
   OperandoILOC *op_destino = cria_operando_registrador(registradorCC1);

   cria_codigo_com_label_e_append(operador, label, lista(op_origem,op_origem2), operacao, op_destino);

   OperandoILOC *cbr_origem = cria_operando_registrador(registradorCC1);
   OperandoILOC *cbr_destino = cria_operando_registrador(label_true);
   OperandoILOC *cbr_destino2 = cria_operando_registrador(label_false);

   cria_codigo_e_append(operador, cbr_origem, CMP_EQ, lista(cbr_destino,cbr_destino2));
}
//#endregion Código 

//#region Instrução 

// loadI c1 => r2 // r2 = c1
CodigoILOC *instrucao_loadI(int valor) {

   OperandoILOC *origem = cria_operando_imediato(valor);

   OperandoILOC *destino = cria_operando_registrador(gera_nome_registrador());

   CodigoILOC *codigo = _cria_codigo(origem, LOADI, destino); //TODO tem q retornar o registrador de alguma formaaaa

   return codigo;
}


// ex: jumpI -> l1 // PC = endereço(l1)
CodigoILOC *instrucao_jump(OperandoILOC* destino) {

   return _cria_codigo(NULL, JUMPI, destino);
}

//#endregion Instrução 