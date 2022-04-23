#include "asm.h"

void generateAsm(Nodo *arvore) {
   asm_inicial();
   asm_seg_dados();
}

//  .file   "etapa6.c"
//  .text
void asm_inicial() {
   printf("\t.file\tetapa6.c\n\t.text\n");  
}

void asm_seg_dados() {
   PilhaHash *esc_global = escopo_global();

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