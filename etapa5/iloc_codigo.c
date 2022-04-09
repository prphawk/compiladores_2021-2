#include "iloc_codigo.h"

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

OperandoCodigo *cria_operando_label(char *nome, int valor) {
   return cria_operando(nome, valor, LABEL);
}

OperandoCodigo *cria_operando_registrador(char* nome) {
   return cria_operando(nome, 0, REGISTRADOR);
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

// loadI c1 => r2 // r2 = c1
void codigo_literal(Nodo *nodo) {

    int valor = nodo->valor_lexico.valor_int; //2.3: Simplificações para a Geração de Código

    char* registrador = gera_nome_registrador();

    OperandoCodigo *origem = cria_operando_imediato(valor);

    OperandoCodigo *destino = cria_operando_registrador(registrador);

    cria_codigo(origem, LOADI, destino);
    
    nodo->codigo = global_codigo;
    //nodo->resultado = destino;
}

// storeAI r0 => r1 (rfp ou rbss),deslocamento
void codigo_atribuicao(Nodo *nodo, int deslocamento, int eh_escopo_global) {
    if (nodo->tipo == TIPO_INT)
    {
        int valor = nodo->valor_lexico.valor_int; //2.3: Simplificações para a Geração de Código
        
        char* registrador = gera_nome_registrador();

        OperandoCodigo *origem = eh_escopo_global ? cria_rbss() : cria_rfp();

        OperandoCodigo *destino = cria_operando_registrador(registrador);
        OperandoCodigo *destino2 = cria_operando_registrador(registrador);
        liga_operandos(destino, destino2);

        cria_codigo(origem, STOREAI, destino);
        
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
   char *registradorResult = gera_nome_registrador();
   char *labelTrue = gera_nome_rotulo();
   char *labelFalse = gera_nome_rotulo();
   char *labelFim = gera_nome_rotulo();

   codigo_logico_auxiliar(nodo, labelTrue, labelFalse);
   //TODO COLOQUE AQUI A LABEL TRUE
   OperandoCodigo *origemTrue = cria_operando_imediato(1);

   OperandoCodigo *destinoTrue = cria_operando_registrador(registradorResult);

   cria_codigo(origemTrue, LOADI, destinoTrue);
    
   //TODO COLOQUE AQUI A LABEL FALSE
   OperandoCodigo *origemFalse = cria_operando_imediato(1);

   OperandoCodigo *destinoFalse = cria_operando_registrador(registradorResult);

   cria_codigo(origemFalse, LOADI, destinoFalse);
   //TODO COLOQUE AQUI A LABEL FIM
   nodo->codigo = global_codigo;
}


void codigo_logico_auxiliar(Nodo *nodo, char* labelTrue, char* labelFalse) {
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
         codigo_logico_operacoes(CMP_EQ, labelTrue, labelFalse);
      break;

      case CMP_NE:
         codigo_logico_operacoes(CMP_NE, labelTrue, labelFalse);
      break;

      case CMP_LE:
         codigo_logico_operacoes(CMP_LE, labelTrue, labelFalse);
      break;

      case CMP_GE:
         codigo_logico_operacoes(CMP_GE, labelTrue, labelFalse);
      break;

      case CMP_GT:
         codigo_logico_operacoes(CMP_GT, labelTrue, labelFalse);
      break;

      case CMP_LT:
         codigo_logico_operacoes(CMP_LT, labelTrue, labelFalse);
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
         codigo_logico_auxiliar(nodo->filho, labelTrue, label1);
         //TODO COLOCA AQUI O LABEL 1
         codigo_logico_auxiliar(nodo->filho->irmao, labelTrue, labelFalse);
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
         codigo_logico_auxiliar(nodo->filho, label1, labelFalse);
         //TODO COLOCA AQUI O LABEL 1
         codigo_logico_auxiliar(nodo->filho->irmao, labelTrue, labelFalse);
      break;
      
      default:
      break;
   }
}

void codigo_logico_operacoes(Operacao operacao, char* labelTrue, char* labelFalse)
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
   OperandoCodigo *cbr_destino = cria_operando_registrador(labelTrue);
   OperandoCodigo *cbr_destino2 = cria_operando_registrador(labelFalse);
   liga_operandos(cbr_destino,cbr_destino2);

   cria_codigo(cbr_origem, CMP_EQ, cbr_destino);
}