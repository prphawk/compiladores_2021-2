#include "tabela_simbolos.h"

PilhaHash *pilha_hash = NULL;

/*
TABELA HASH - a tabela hash é pra ser construida utilizando open adressing. 
              ela é um array de estruturas EntradaHash alocado dinamicamente (não confundir com linked lists).
              isso nos dá a opção de expandir o tamanho da tabela se ficar mt densa. é necessário guardar o tamanho do array alocado assim.
CONFLITOS   - o probing em caso de conflito pula pro endereço seguinte. por enquanto ele só dá um pulinho de cada vez. dá pra mudar.
PILHA       - uma estrutura da pilha guarda algumas informações da tabela e a tabela em si. a pilha global é formada por uma linked list.
*/

//#region Auxiliares
//TODO função que retorna o tamanho do tipoSimbolo ISSO AI TA INCOMPLETO!!!!
int tamanho(TipoSimbolo tipo) {
    switch(tipo) {
        case TIPO_INTEIRO: return TAMANHO_INT; break;
        case TIPO_FLOAT: return TAMANHO_FLOAT; break;
        case TIPO_BOOL:
        case TIPO_STRING:
        case TIPO_CHAR: return TAMANHO_CHAR; break;
        default: return -1; break;
    }
}

// função que recebe um nome (label?) e uma natureza (variável, função, literal) e cria uma chave dando append nos dois
// exemplo out: func1_nat_3_tipo_1
// TODO botar mais info nesse nome viu. quem sabe o tipo tbm. e concatenar mais q numeros só
char *chave(char *nome, NaturezaSimbolo natureza, TipoSimbolo tipo)
{
    char str[48];
    sprintf(str, "_nat_%d_t_%d", natureza, tipo);
    return append_str_malloc(nome, str);
}

// função que retorna um valor hash a partir da chave
unsigned long indice_hash(char *chave)
{
    unsigned char *str = chave;
    // usa o chave() aqui dentro -> what? a entrada eh a chave já

    // djb2 (1991) por Daniel J. Bernstein, explicação em https://theartincode.stanis.me/008-djb2/
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int probing(int indice, int tamanho_tabela) {

    indice++;
		
    indice %= tamanho_tabela;

    return indice;
}

// TODO expandir tamanho da tabela quando atinge 75% da capacidade
PilhaHash *expande_tabela(PilhaHash *pilha)
{
   return NULL;        
}

// TODO função que adiciona um argumento à lista de argumentos de uma variável > DO TIPO FUNÇÃO < (checar!!!)
void adiciona_argumento(EntradaHash entrada, TipoSimbolo tipo, int tamanho, ValorLexico valor_lexico)
{
    // não precisa usar o "chave", guarda somente o >NOME< do argumento em argumento.nome
    // cria um entradaArgumento
    return;
}
//#endregion Auxiliares

//#region Busca
//  função que retorna uma entrada específica da hash a partir de sua chave
EntradaHash *encontra_no_escopo(char *chave, PilhaHash *pilha)
{
    if(pilha == NULL) return NULL;
    
    EntradaHash *resposta = encontra_na_tabela(chave, pilha->topo, pilha->capacidade_tabela);

    if(resposta != NULL) return resposta;
    
    PilhaHash *resto = (PilhaHash*)pilha->resto;
	
    return encontra_no_escopo(chave, resto);
}

EntradaHash *encontra_na_tabela(char *chave, EntradaHash *tabela, int tamanho_tabela) {

    int indice = indice_hash(chave) % tamanho_tabela;  

    while(tabela != NULL) {

        if(tabela[indice].chave == chave)
            return &tabela[indice]; 
            
        indice = probing(indice, tamanho_tabela);
    }

    return NULL;
}
//#endregion Busca

//#region Insere
// função que "empilha" uma nova hash em cima da atual
void empilha()
{
    PilhaHash *pilha_aux;
    pilha_aux = (PilhaHash*)malloc(sizeof(PilhaHash));

    pilha_aux->quantidade_atual = 0;
    pilha_aux->capacidade_tabela = TAMANHO_INICIAL_HASH;

    EntradaHash *tabela = adiciona_tabela();
    pilha_aux->topo = tabela;
    pilha_aux->resto = (struct PilhaHash*)pilha_hash;

    pilha_hash = pilha_aux;
}

//aloca novo array de EntradaHash (com valores NULL pls)
EntradaHash *adiciona_tabela() {

    EntradaHash *tabela = (EntradaHash*)malloc(sizeof(EntradaHash) * TAMANHO_INICIAL_HASH);

    for (int i = 0; i < TAMANHO_INICIAL_HASH; i++) {
        EntradaHash* entrada = &tabela[i];
        entrada->chave = NULL;
        entrada->conteudo.tipo = -1;
        entrada->conteudo.natureza = -1;
        entrada->conteudo.linha = -1;
        entrada->conteudo.coluna = -1;
        entrada->conteudo.tamanho = -1;
        entrada->conteudo.argumentos = NULL;
    }

    return tabela;
}

void insere_literal_tabela(TipoSimbolo tipo, ValorLexico valor_lexico) {
    insere_no_escopo(NATUREZA_LITERAL, tipo, valor_lexico);
}
void insere_funcao_tabela(ValorLexico valor_lexico) {
    insere_no_escopo(NATUREZA_FUNCAO, TIPO_STRING, valor_lexico);
}
void insere_identificador_tabela(TipoSimbolo tipo, ValorLexico valor_lexico) {
    insere_no_escopo(NATUREZA_VARIAVEL, tipo, valor_lexico);
}

// função que adiciona uma entrada na hash e retorna a recém-adicionada entrada
EntradaHash *insere_no_escopo(NaturezaSimbolo natureza, TipoSimbolo tipo, ValorLexico valor_lexico) {

    if(pilha_hash == NULL) empilha();

    PilhaHash* pilha = pilha_hash;

    if(pilha == NULL) return NULL;

    char *chave_malloc = chave(valor_lexico.label, natureza, tipo);

    Conteudo conteudo;
    conteudo.linha = valor_lexico.linha;
    conteudo.coluna = -1;
    conteudo.tamanho = tamanho(tipo);
    conteudo.tipo = tipo;
    conteudo.natureza = natureza;
    conteudo.argumentos = NULL;
    conteudo.valor_lexico = valor_lexico;

    insere_na_tabela(chave_malloc, pilha, conteudo);
}

EntradaHash *insere_na_tabela(char *chave, PilhaHash *pilha, Conteudo conteudo) {

    EntradaHash *tabela = pilha->topo;

    if(tabela == NULL) return NULL;

    int tamanho_tabela = pilha->capacidade_tabela;

    int indice = indice_hash(chave) % tamanho_tabela;  

    while(tabela != NULL) {

        if(tabela[indice].chave == NULL) {

            tabela[indice].chave = chave;
            tabela[indice].conteudo = conteudo;

            pilha->quantidade_atual++; 

            return &tabela[indice]; 
        }

        indice = probing(indice, tamanho_tabela);
    }
}
//#endregion Insere

//#region Libera 

void libera_pilha() {
    while(pilha_hash != NULL) {
        desempilha();
    }
}

// função que "desempilha" (elimina) a hash e suas informações no topo da pilha
void desempilha()
{
    if(pilha_hash == NULL) return;

    PilhaHash *antiga_pilha;
    
    antiga_pilha = pilha_hash;

    PilhaHash *nova_pilha = (PilhaHash *)antiga_pilha->resto;

    if(nova_pilha != NULL) {
        pilha_hash->topo = nova_pilha->topo;
        pilha_hash->resto = nova_pilha->resto;
        pilha_hash->capacidade_tabela = nova_pilha->capacidade_tabela;
        pilha_hash->quantidade_atual = nova_pilha->quantidade_atual;
    } else {
        pilha_hash = NULL;
    }

    libera_tabela(antiga_pilha->topo, antiga_pilha->capacidade_tabela);

    free(antiga_pilha);
}

// função que libera a tabela hash e tudo que há dentro dela (i.e. libera a memória caso necessário)
void libera_tabela(EntradaHash *tabela, int tamanho_tabela) {

    if(tabela == NULL) return;

    for (int i = 0; i < tamanho_tabela; i++) {

        if(tabela[i].chave != NULL) {

            free(tabela[i].chave);

            libera_argumentos(tabela[i].conteudo.argumentos);
        }
    }

    free(tabela);
}

void libera_argumentos(ArgumentoFuncao *argumento) {

    if(argumento == NULL) return;

    libera_argumentos((ArgumentoFuncao *)argumento->proximo);

    free(argumento);
}
//#endregion Libera 

//#region Prints
void print_escopos() {

    PilhaHash *aux_pilha = (PilhaHash*)pilha_hash;

    int profundidade = 1;

    while(aux_pilha != NULL) {

        printf("\nESCOPO Nº%i DA PILHA | CAPACIDADE: %3i | OCUPAÇÃO: %i -----\n\n", profundidade, aux_pilha->capacidade_tabela, aux_pilha->quantidade_atual);

        print_tabela(aux_pilha->topo, aux_pilha->capacidade_tabela);

        printf("\n----------------------------------------------------------\n");

        aux_pilha = (PilhaHash*)aux_pilha->resto;

        profundidade++;
    }
}

//TODO printa pilha com tabela e seus valores (tem que botar mais infos ai)
void print_tabela(EntradaHash *tabela, int tamanho) {
    for(int i=0; i < tamanho; i++) {
       EntradaHash entrada = tabela[i];
       printf("ITEM %3i | TIPO %s | NATUREZA %s | CHAVE %s\n", i+1, print_natureza(entrada.conteudo.natureza), print_tipo(entrada.conteudo.tipo), entrada.chave);
    }
}

char* print_tipo(TipoSimbolo tipo) {
    switch(tipo) {
        case TIPO_INTEIRO: return "INT   "; break;
        case TIPO_FLOAT: return   "FLOAT "; break;
        case TIPO_BOOL: return    "BOOL  "; break;
        case TIPO_STRING: return  "STRING"; break;
        case TIPO_CHAR: return    "CHAR  "; break;
        case TIPO_OUTRO: return   "OUTRO "; break;
        default: return           "------"; break;
    }
}

char* print_natureza(NaturezaSimbolo natureza) {
    switch(natureza) {
        case NATUREZA_FUNCAO: return   "FUNCAO  "; break;
        case NATUREZA_LITERAL: return  "LITERAL "; break;
        case NATUREZA_VARIAVEL: return "VARIAVEL"; break;
        default: return                "--------"; break;
    }
}
//#endregion Prints