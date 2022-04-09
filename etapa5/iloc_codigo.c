#include "iloc_codigo.h"
#include "tabela_simbolos.h"

CodigoILOC *global_codigo = NULL;

void cria_codigo(OperandoCodigo *origem, Operacao operacao, OperandoCodigo *destino)
{
    CodigoILOC *codigo = malloc(sizeof(CodigoILOC));
    codigo->origem = origem;
    codigo->operacao = operacao;
    codigo->destino = destino;

    codigo->anterior = global_codigo;
    global_codigo = codigo;
}

OperandoCodigo *cria_operando(char* nome, int valor, TipoOperando tipo) {
    OperandoCodigo *operando = malloc(sizeof(OperandoCodigo));
    operando->nome = nome;
    operando->valor = valor;
    operando->tipo = tipo;
    operando->proximo = NULL;
    return operando;
}

OperandoCodigo *cria_operando_imediato(int valor) {
   return cria_operando(NULL, valor, IMEDIATO);
}

OperandoCodigo *cria_operando_registrador(char* nome) {
   return cria_operando(nome, 0, REGISTRADOR);
}

OperandoCodigo *cria_operando_label(char* nome) {
   return cria_operando(nome, 0, LABEL);
}

OperandoCodigo *cria_operando_registrador_especial(char* nome) {
   return cria_operando(nome, 0, REGISTRADOR_ESPECIAL);
}

OperandoCodigo *cria_rfp() {
   return cria_operando_registrador(RFP);
}

OperandoCodigo *cria_rsp() {
   return cria_operando_registrador(RSP);
}

OperandoCodigo *cria_rbss() {
   return cria_operando_registrador(RBSS);
}

OperandoCodigo *cria_rpc() {
   return cria_operando_registrador(RPC);
}

void liga_operandos(OperandoCodigo *primeiro, OperandoCodigo *segundo) 
{
    primeiro->proximo = segundo;
}

void desliga_operando(OperandoCodigo *primeiro)
{
    primeiro->proximo = NULL;
}

//loadAI originRegister, originOffset => resultRegister // r3 = Memoria(r1 + c2)
void codigo_carrega_variavel(Nodo *nodo) {

   DeslocamentoEscopo busca = busca_deslocamento_e_escopo(nodo->valor_lexico.label);

   int deslocamento = busca.deslocamento;

   OperandoCodigo *origem_1_registrador = busca.eh_escopo_global ? cria_rbss() : cria_rfp();
   OperandoCodigo *origem_2_deslocamento = cria_operando_imediato(deslocamento);

   origem_1_registrador->proximo = origem_2_deslocamento;

   OperandoCodigo *destino = cria_operando_registrador(gera_nome_registrador());

   cria_codigo(origem_1_registrador, LOADAI, destino);

   //TODO e o codigo de nodo? n tem na referencia nodo->codigo = global_codigo;
   nodo->resultado = destino;
}

// loadI c1 => r2 // r2 = c1
void codigo_carrega_literal(Nodo *nodo) {

   int valor = nodo->valor_lexico.valor_int; //2.3: Simplificações para a Geração de Código

   char* registrador = gera_nome_registrador();

   OperandoCodigo *origem = cria_operando_imediato(valor);

   OperandoCodigo *destino = cria_operando_registrador(registrador);

   cria_codigo(origem, LOADI, destino);
   
   nodo->codigo = global_codigo;
   nodo->resultado = destino;
}

// storeAI r0 => r1 (rfp ou rbss),deslocamento
void codigo_atribuicao(Nodo *nodo) {

    if (nodo->tipo == TIPO_INT) {

        int valor = nodo->valor_lexico.valor_int; //2.3: Simplificações para a Geração de Código

		DeslocamentoEscopo busca = busca_deslocamento_e_escopo(nodo->valor_lexico.label);
        
        OperandoCodigo *origem = cria_operando_registrador(gera_nome_registrador()); 

        OperandoCodigo *destino_1_ponteiro = busca.eh_escopo_global ? cria_rbss() : cria_rfp();
		OperandoCodigo *destino_2_deslocamento = cria_operando_imediato(busca.deslocamento);

        liga_operandos(destino_1_ponteiro, destino_2_deslocamento);

        cria_codigo(origem, STOREAI, destino_1_ponteiro);
        
        nodo->codigo = global_codigo;
        //nodo->resultado = destino;
    }
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

   codigo_logico_auxiliar(nodo, label_true, label_fim);
   //TODO COLOQUE AQUI A LABEL TRUE
   OperandoCodigo *origem_load_true = cria_operando_imediato(1);

   OperandoCodigo *destino_load_true = cria_operando_registrador(registrador_result);

   cria_codigo(origem_load_true, LOADI, destino_load_true);

   OperandoCodigo *destino_jump_true = cria_operando_label(label_fim);

   cria_codigo(NULL, JUMPI, destino_jump_true);
    
   //TODO COLOQUE AQUI A LABEL FALSE
   OperandoCodigo *origem_load_false = cria_operando_imediato(0);

   OperandoCodigo *destino_load_false = cria_operando_registrador(registrador_result);

   cria_codigo(origem_load_false, LOADI, destino_load_false);

   OperandoCodigo *destino_jump_false = cria_operando_label(label_fim);

   cria_codigo(NULL, JUMPI, destino_jump_false);
   //TODO COLOQUE AQUI A LABEL FIM
   nodo->codigo = global_codigo;
}


void codigo_logico_auxiliar(Nodo *nodo, char* label_true, char* label_false) {
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
         codigo_logico_operacoes(CMP_EQ, label_true, label_false);
      break;

      case CMP_NE:
         codigo_logico_operacoes(CMP_NE, label_true, label_false);
      break;

      case CMP_LE:
         codigo_logico_operacoes(CMP_LE, label_true, label_false);
      break;

      case CMP_GE:
         codigo_logico_operacoes(CMP_GE, label_true, label_false);
      break;

      case CMP_GT:
         codigo_logico_operacoes(CMP_GT, label_true, label_false);
      break;

      case CMP_LT:
         codigo_logico_operacoes(CMP_LT, label_true, label_false);
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
         codigo_logico_auxiliar(nodo->filho, label_true, label1);
         //TODO COLOCA AQUI O LABEL 1
         codigo_logico_auxiliar(nodo->filho->irmao, label_true, label_false);
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
         codigo_logico_auxiliar(nodo->filho, label1, label_false);
         //TODO COLOCA AQUI O LABEL 1
         codigo_logico_auxiliar(nodo->filho->irmao, label_true, label_false);
      break;
      
      default:
      break;
   }
}

void codigo_logico_operacoes(Operacao operacao, char* label_true, char* label_false)
{
   char *registradorA = gera_nome_registrador();
   char *registradorB = gera_nome_registrador();
   char *registradorCC1 = gera_nome_registrador();

   OperandoCodigo *op_origem = cria_operando_registrador(registradorA);
   OperandoCodigo *op_origem2 = cria_operando_registrador(registradorB);
   liga_operandos(op_origem,op_origem2);
   OperandoCodigo *op_destino = cria_operando_registrador(registradorCC1);

   cria_codigo(op_origem, operacao, op_destino);

   OperandoCodigo *cbr_origem = cria_operando_registrador(registradorCC1);
   OperandoCodigo *cbr_destino = cria_operando_registrador(label_true);
   OperandoCodigo *cbr_destino2 = cria_operando_registrador(label_false);
   liga_operandos(cbr_destino,cbr_destino2);

   cria_codigo(cbr_origem, CMP_EQ, cbr_destino);
}

// ex: jumpI -> l1 // PC = endereço(l1)
CodigoILOC *_codigo_jump(char* label_destino) {

   OperandoCodigo *destino = cria_operando_label(label_destino);

   cria_codigo(NULL, JUMPI, destino);
}