#include "ast.h"
#include "tabela_simbolos.h"

void generateAsm(Nodo *arvore);
void asm_inicial();
void asm_seg_dados();
void asm_seg_dados_funcao(char* nome);
void asm_seg_dados_variavel(char* nome);

void asm_seg_codigo(CodigoILOC *codigo);
void asm_seg_codigo_com_tipo(CodigoILOC *codigo);
void asm_seg_codigo_ops(CodigoILOC *codigo);
void asm_funcao_inicio(CodigoILOC *codigo);
void asm_funcao_fim();

void ops_loadi(CodigoILOC *codigo);
void ops_storeai(CodigoILOC *codigo);

void _empilha_literal(int num);
void _desempilha_para_dest(char* dest);