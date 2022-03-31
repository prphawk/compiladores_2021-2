#include "tabela_simbolos.h"
#include "errors.h"

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
EntradaHash *busca_pilha(char *chave, PilhaHash *pilha)
{
    if(pilha == NULL) return NULL;
    
    EntradaHash *resposta = busca_no_topo_pilha(chave, pilha);

    if(resposta != NULL) return resposta;
    
    PilhaHash *resto = (PilhaHash*)pilha->resto;
	
    return busca_pilha(chave, resto);
}

EntradaHash *busca_no_topo_pilha(char *chave, PilhaHash *pilha) {

    EntradaHash *tabela = pilha->topo;

    int tamanho_tabela = pilha->capacidade_tabela;

    int indice = indice_hash(chave) % tamanho_tabela;  

    while(tabela[indice].chave != NULL) {

        if(compare_eq_str(tabela[indice].chave, chave))
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

    EntradaHash *tabela = nova_tabela();
    pilha_aux->topo = tabela;
    pilha_aux->resto = (struct PilhaHash*)pilha_hash;

    pilha_hash = pilha_aux;
}

//aloca novo array de EntradaHash (com valores NULL pls)
EntradaHash *nova_tabela() {

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
    insere_na_pilha(NATUREZA_LITERAL, tipo, valor_lexico);
}
void insere_funcao_tabela(ValorLexico valor_lexico) {
    insere_na_pilha(NATUREZA_FUNCAO, TIPO_STRING, valor_lexico);
}
void insere_identificador_tabela(TipoSimbolo tipo, ValorLexico valor_lexico) {
    insere_na_pilha(NATUREZA_VARIAVEL, tipo, valor_lexico);
}

// função que adiciona uma entrada na hash e retorna a recém-adicionada entrada
// chamar ao declarar
EntradaHash *insere_na_pilha(NaturezaSimbolo natureza, TipoSimbolo tipo, ValorLexico valor_lexico) {

    if(pilha_hash == NULL) empilha();

    PilhaHash* pilha = pilha_hash;

    if(pilha == NULL) return NULL;

    char *chave_malloc = chave(valor_lexico.label, natureza, tipo);

    EntradaHash *resposta = busca_no_topo_pilha(chave_malloc, pilha);

    if(resposta != NULL) {
        //TODO ve redlecaração ou apenas atribuição??
        // throwDeclaredError(valor_lexico.linha, valor_lexico.label, resposta->conteudo.linha);
        return NULL;
    }

    Conteudo conteudo;
    conteudo.linha = valor_lexico.linha;
    conteudo.coluna = -1;
    conteudo.tamanho = tamanho(tipo);
    conteudo.tipo = tipo;
    conteudo.natureza = natureza;
    conteudo.argumentos = NULL;
    conteudo.valor_lexico = valor_lexico;

    insere_na_pilha_probing(chave_malloc, pilha, conteudo);

    print_escopos();
    printf("(OUTRA INTERAÇÃO) ____________________________________________________________________\n");
}

//TODO checar se já existe
EntradaHash *insere_na_pilha_probing(char *chave, PilhaHash *pilha, Conteudo conteudo) {

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

    PilhaHash *aux_pilha = pilha_hash;

    int profundidade = 1;

    while(aux_pilha != NULL) {

        int capacidade = aux_pilha->capacidade_tabela;

        printf("\n\n - ESCOPO Nº%02i DA PILHA - | CAPACIDADE: %i | OCUPAÇÃO: %03i\n", profundidade, capacidade, aux_pilha->quantidade_atual);
        printf("--------------------------------------------------------------------\n");
        print_tabela(aux_pilha->topo, capacidade);
        printf("--------------------------------------------------------------------\n\n");

        profundidade++;

        aux_pilha = (PilhaHash*)aux_pilha->resto;
    }
}

//printa pilha com tabela e seus valores
void print_tabela(EntradaHash *tabela, int tamanho) {
    for(int i=0; i < tamanho; i++) {
       EntradaHash entrada = tabela[i];
        
        if(entrada.chave == NULL) continue;

       printf(" ITEM %03i | TIPO %s | NATUREZA %s | CHAVE %s\n", i+1, print_natureza(entrada.conteudo.natureza), print_tipo(entrada.conteudo.tipo), entrada.chave);
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