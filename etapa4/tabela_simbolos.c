#include "tabela_simbolos.h"

PilhaHash *pilha_hash = NULL;

// função que recebe um nome (label?) e uma natureza (variável, função, literal) e cria uma chave dando append nos dois
// exemplo out: nomevar1
char *chave(char *nome, NaturezaSimbolo natureza)
{
    char str[8];
    sprintf(str, "%d", natureza);
    return append_str_malloc(nome, str);
    //free dps!!!
}

// função que retorna um valor hash a partir da chave
unsigned long indice_hash(char *chave)
{
    unsigned char *str = chave;
    // usa o chave() aqui dentro -> what? a entrada eh a chave já

    // djb2 (1991) by Daniel J. Bernstein, explanation on https://theartincode.stanis.me/008-djb2/
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void insere_literal_tabela(TipoSimbolo tipo, ValorLexico valor_lexico) {
    insere_no_escopo(SIMBOLO_LITERAL, tipo, valor_lexico);
}
void insere_funcao_tabela(ValorLexico valor_lexico) {
    insere_no_escopo(SIMBOLO_FUNCAO, SIMBOLO_TIPO_STRING, valor_lexico);
}
void insere_identificador_tabela(TipoSimbolo tipo, ValorLexico valor_lexico) {
    insere_no_escopo(SIMBOLO_VARIAVEL, tipo, valor_lexico);
}

// TODO função que adiciona uma entrada na hash
// retorna a recém-adicionada entrada
EntradaHash *insere_no_escopo(NaturezaSimbolo natureza, TipoSimbolo tipo, ValorLexico valor_lexico) {

    if(pilha_hash == NULL) empilha();

    PilhaHash* pilha = pilha_hash;

    if(pilha == NULL) return NULL;

    char *chave_malloc = chave(valor_lexico.label, natureza);

    Conteudo conteudo;
    conteudo.linha = valor_lexico.linha;
    conteudo.coluna = -1;
    conteudo.tamanho = tamanho(tipo);
    conteudo.tipo_simbolo = tipo;
    conteudo.natureza_simbolo = natureza;
    conteudo.argumentos = NULL;
    conteudo.valor_lexico = valor_lexico;

    insere_na_tabela(chave_malloc, pilha, conteudo);
    free(chave_malloc);
}

EntradaHash *insere_na_tabela(char *chave, PilhaHash *pilha, Conteudo conteudo) {

    EntradaHash *tabela = pilha->topo;

    if(tabela == NULL) return NULL;

    int tamanho_tabela = pilha->capacidade_tabela;

    int indice = indice_hash(chave) % tamanho_tabela;  

    while(tabela != NULL) {

        if(tabela[indice].chave == NULL) {
            tabela[indice].chave = chave;
            printf("-> chave %s\n", tabela[indice].chave);
            tabela[indice].conteudo = conteudo;
            pilha->quantidade_atual++; 
            return &tabela[indice]; 
        }
        indice = probing(indice, tamanho_tabela);
    }
}

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

//TODO aloca novo array de EntradaHash (com valores NULL pls)
EntradaHash *nova_tabela() {

    EntradaHash *tabela = (EntradaHash*)malloc(sizeof(EntradaHash) * TAMANHO_INICIAL_HASH);

    for (int i = 0; i < TAMANHO_INICIAL_HASH; i++) {
        tabela[i].chave = NULL;
    }

    return tabela;
}

void libera_pilha() {
    while(pilha_hash != NULL) {
        desempilha();
    }
}

// TODO função que "empilha" uma nova hash em cima da atual
void empilha()
{
    PilhaHash *pilha_aux;
    pilha_aux = (PilhaHash*)malloc(sizeof(PilhaHash));

    pilha_aux->quantidade_atual = 0;
    pilha_aux->capacidade_tabela = TAMANHO_INICIAL_HASH;

    EntradaHash *tabela = nova_tabela();
    pilha_aux->topo = tabela;
    pilha_aux->resto = (struct PilhaHash*)pilha_hash;

    pilha_hash = pilha_aux;
}

// TODO função que "desempilha" (elimina) a hash no topo da pilha
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

// TODO função que libera a tabela hash e tudo que há dentro dela (i.e. libera a memória caso necessário)
void libera_tabela(EntradaHash *tabela, int tamanho_tabela) {

    for (int i = 0; i < tamanho_tabela; i++) {

        if(tabela[i].chave != NULL) {

            //free(tabela[i].chave);

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

//TODO função que retorna o tamanho do tipoSimbolo ISSO AI TA INCOMPLETO!!!!
int tamanho(TipoSimbolo tipo) {
    switch(tipo) {
        case SIMBOLO_TIPO_INTEIRO: return TAMANHO_INT; break;
        case SIMBOLO_TIPO_FLOAT: return TAMANHO_FLOAT; break;
        case SIMBOLO_TIPO_BOOL:
        case SIMBOLO_TIPO_STRING:
        case SIMBOLO_TIPO_CHAR: return TAMANHO_CHAR; break;
        default: return -1; break;
    }
}

//TODO printa pilha com tabela e seus valores
void print_escopos() {

    PilhaHash *aux_pilha = (PilhaHash*)pilha_hash;

    int profundidade = 0;

    while(aux_pilha != NULL) {

        print_tabela(profundidade, aux_pilha->topo, aux_pilha->capacidade_tabela);

        aux_pilha = (PilhaHash*)aux_pilha->resto;

        profundidade++;
    }

}

void print_tabela(int profundidade, EntradaHash *tabela, int tamanho) {
    printf("\n\nPROFUNDIDADE %i ---------------------\n\n", profundidade);

    for(int i=0; i < tamanho; i++) {
       EntradaHash entrada = tabela[i];
       printf("ITEM %i | CHAVE %s | TIPO %i\n", i, entrada.chave, entrada.conteudo.tipo_simbolo);
    }

    printf("\n----------------------------------------\n");
}