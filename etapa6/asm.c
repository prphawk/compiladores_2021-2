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

//  .file   "etapa6.c"
//  .text
void asm_inicial() {
   printf("\t.file\tetapa6.c\n");
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

void asm_seg_codigo(CodigoILOC *codigo) {

   // inicio de funcao, label com NOP
   switch (codigo->tipo_cod)
   {
      case cod_decl_func:
         asm_funcao_inicio(codigo); break;
      case cod_fim_func:
         asm_funcao_fim(); break;
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
   printf("ret\n");
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