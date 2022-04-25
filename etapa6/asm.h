#include "ast.h"
#include "tabela_simbolos.h"

void generateAsm(Nodo *arvore);
void asm_inicial();
void asm_seg_dados();
void asm_seg_dados_funcao(char* nome);
void asm_seg_dados_variavel(char* nome);

void asm_seg_codigo(CodigoILOC *codigo);
void asm_funcao_inicio(CodigoILOC *codigo);
void asm_funcao_fim();