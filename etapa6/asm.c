#include "asm.h"

/*
The data section is used for declaring initialized data or constant. This data does not change at runtim
   -> Não temos globais inicializadas!

Data Registers: Four 32-bit data registers are used for arithmetic, logical and other operations. These 32-bit registers can be used
in three ways: 1. As complete 32-bit data registers: EAX, EBX, ECX, EDX.
*/

void generateAsm(Nodo *arvore) {
   asm_inicial();
   asm_seg_dados();
   asm_seg_codigo(arvore->codigo);
}

void asm_seg_codigo(CodigoILOC *codigo) {

   if(codigo == NULL) return;

   asm_seg_codigo(codigo->anterior);

   if(codigo->tipo_cod != cod_null)
      asm_seg_codigo_com_tipo(codigo);
   else asm_seg_codigo_ops(codigo);
}

//  .file   "etapa6.c"
//  .text
void asm_inicial() {
   printf("\t.file\t\"etapa6.c\"\n");
   printf("\t.text\n");
}

void asm_seg_dados() {
   PilhaHash *esc_global = get_escopo_global();

   if(esc_global == NULL) return;

   for(int i = 0; i < esc_global->tamanho_tabela; i++) {

      EntradaHash elem = esc_global->topo[i];
      if(elem.chave == NULL) continue;

      char* nome = elem.conteudo.valor_lexico.label;

      if(elem.conteudo.natureza == NATUREZA_VARIAVEL)
         asm_seg_dados_variavel(nome);
      else if(elem.conteudo.natureza == NATUREZA_FUNCAO)
         asm_seg_dados_funcao(nome);
   }
}

void asm_seg_codigo_com_tipo(CodigoILOC *codigo) {

   switch (codigo->tipo_cod)
   {
      case cod_decl_func:
         asm_funcao_inicio(codigo); break;
      case cod_fim_func:
         asm_funcao_fim(); break;
      case cod_return: 
         
      break;
      case cod_skip: break;
      default: break;
   }
}
void asm_seg_codigo_ops(CodigoILOC *codigo) {

   switch (codigo->operacao)
   {
      case LOADI: ops_loadi(codigo);
      case STOREAI: ops_storeai(codigo);
      default: break;
   }
}

/*
label:
   pushq	%rbp
   movq	%rsp, %rbp
*/
void asm_funcao_inicio(CodigoILOC *codigo) {
   printf("%s:\n", codigo->label);
   printf("\tpushq\t%%rbp\n");
   printf("\tmovq\t%%rsp, %%rbp\n");
}

/*
   popq	%rbp
   ret
*/
void asm_funcao_fim() {
   printf("\tpopq\t%%rbp\n");
   printf("\tret\n");
}

/* .comm name, size, alignment.
The '.comm' directive allocates storage in the data section. 
The storage is referenced by the identifier name. 
Size is measured in bytes and must be a positive integer. 
Name cannot be predefined. Alignment is optional. */
//  .comm   nome,4
void asm_seg_dados_variavel(char* nome) {
   printf("\t.comm\t%s,4\n", nome);
}

//  .globl  nome
//  .type   nome, @function
void asm_seg_dados_funcao(char* nome) {
   printf("\t.globl\t%s\n", nome);
   printf("\t.type\t%s, @function\n", nome);
}

void ops_loadi(CodigoILOC *codigo) {
   OperandoILOC *operando = codigo->origem;
   if(operando && operando->tipo == IMEDIATO)
      _empilha_literal(operando->valor);
}

void ops_storeai(CodigoILOC *codigo) {

   OperandoILOC *op_destino = codigo->destino;
   
   if(op_destino == NULL || op_destino->proximo == NULL) return;
   
   _desempilha_para_dest("%eax");
   printf("\tmovl\t%%eax, -%i(%%rbp)\n", (op_destino->proximo->valor + 4));
}

/*
%rsp is special: it is the “stack pointer”, used to maintain the function call stack. 
It points to the bottom of the stack, which grows “down” (toward lower addresses) on x86. 
So, our subq $24, %rsp instruction is making space for three 64-bit integers on the stack. 

   subq  $4, %rsp       
   movl  $<num>, %rsp      ; espaço pra empilhar o literal
*/
void _empilha_literal(int num) {

   printf("\tsubq\t$4, (%%rsp)\n"); //TODO talvez seja 8?
   printf("\tmovl\t$%i, (%%rsp)\n", num);
}

/*
   movl  (%%rsp), <dest>
   addq  $4, %%rsp
*/
void _desempilha_para_dest(char* dest) {
   printf("\tmovl\t(%%rsp), %s\n", dest);
   printf("\taddq\t$4, (%%rsp)\n");
}