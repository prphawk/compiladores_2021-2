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

//TODO função que retorna o tamanho do tipoSimbolo ISSO AI TA INCOMPLETO!!!! tem q calcular com o tamanho da string tbm
int _tamanho(ValorLexico valor_lexico, TipoSimbolo tipo) {
    switch(tipo) {
        case TIPO_INT: return TAMANHO_INT; break;
        case TIPO_FLOAT: return TAMANHO_FLOAT; break;
        case TIPO_STRING: return strlen(valor_lexico.valor_string)*TAMANHO_CHAR; break;
        case TIPO_BOOL:
        case TIPO_CHAR: return TAMANHO_CHAR; break;
        default: return -1; break;
    }
}

// função que recebe um nome (label?) e uma natureza (variável, função, literal) e cria uma chave dando append nos dois
// exemplo out: func1_n_3
// TODO ver se esse projeto é pra aceitar a declaração do mesmo nome com naturezas diferentes. tipo nome (variavel) != nome(){} (função)
char *_chave(char *nome, NaturezaSimbolo natureza)
{
    char str[10];
    sprintf(str, "_n_%d", natureza);

    return append_str_malloc(nome, str);
}

// função que retorna um valor hash a partir da chave
unsigned long _indice_hash(char *chave)
{
    unsigned char *str = chave;

    // djb2 (1991) por Daniel J. Bernstein, explicação em https://theartincode.stanis.me/008-djb2/
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int _probing(int indice, int tamanho_tabela) {

    indice++;
		
    indice %= tamanho_tabela;

    return indice;
}

// TODO expandir tamanho da tabela quando atinge 70% da capacidade
PilhaHash *_malloc_expande_tabela(PilhaHash *pilha)
{
   return NULL;        
}

// TODO função que adiciona um argumento à lista de argumentos de uma variável > DO TIPO FUNÇÃO < (checar!!!)
// TODO cara como isso bate com a declaração dos parametros? como se bota uma função na tabela what
// vai ter q ficar com os argumentos pendentes tbm
void _adiciona_argumento(EntradaHash entrada, TipoSimbolo tipo, int tamanho, ValorLexico valor_lexico)
{
    //ArgumentoFuncaoLst *argumentoFuncaoLst;
    //argumentoFuncaoLst = malloc(sizeof(ArgumentoFuncaoLst));
    // não precisa usar o "chave", guarda somente o >NOME< do argumento em argumento.nome
    // cria um entradaArgumento
    return;
}
//#endregion Auxiliares

//#region Busca

//  função que retorna uma entrada específica da hash a partir de sua chave
EntradaHash *_busca_pilha(char *chave, PilhaHash *pilha)
{
    if(pilha == NULL) return NULL;
    
    EntradaHash *resposta = _busca_topo_pilha(chave, pilha);

    if(resposta != NULL) return resposta;
    
    PilhaHash *resto = (PilhaHash*)pilha->resto;
	
    return _busca_pilha(chave, resto);
}

EntradaHash *_busca_topo_pilha(char *chave, PilhaHash *pilha) {

    EntradaHash *tabela = pilha->topo;

    int tamanho_tabela = pilha->tamanho_tabela;

    int indice = _indice_hash(chave) % tamanho_tabela;  

    while(tabela[indice].chave != NULL) {

        if(compare_eq_str(tabela[indice].chave, chave))
            return &tabela[indice]; 
            
        indice = _probing(indice, tamanho_tabela);
    }

    return NULL;
}
//#endregion Busca

//#region Insere

void insere_literal_pilha(TipoSimbolo tipo, ValorLexico valor_lexico) {
    _insere_em_pilha(NATUREZA_LITERAL, tipo, valor_lexico);
}
void insere_funcao_pilha(ValorLexico valor_lexico) {
    _insere_em_pilha(NATUREZA_FUNCAO, TIPO_OUTRO, valor_lexico);
}
void insere_identificador_pilha(TipoSimbolo tipo, ValorLexico valor_lexico) {
    _insere_em_pilha(NATUREZA_VARIAVEL, tipo, valor_lexico);
}

void insere_identificador_global_sem_tipo_pilha(ValorLexico valor_lexico, int tamanho_vetor) {

    char* chave = _chave(valor_lexico.label, NATUREZA_VARIAVEL);

    _adiciona_variavel_sem_tipo_pilha(chave, tamanho_vetor);
    _insere_em_pilha(NATUREZA_VARIAVEL, TIPO_PENDENTE, valor_lexico);
}

void _adiciona_variavel_sem_tipo_pilha(char* chave, int tamanho_vetor) {

    VariavelSemTipoLst *nova_vst;
    nova_vst = malloc(sizeof(VariavelSemTipoLst));
    nova_vst->chave = chave;
    nova_vst->tamanho_vetor = tamanho_vetor;

    if(pilha_hash == NULL) empilha();

    VariavelSemTipoLst *vst_head = pilha_hash->variaveis_sem_tipo;
    nova_vst->proximo = (struct VariavelSemTipoLst *)vst_head;
    pilha_hash->variaveis_sem_tipo = nova_vst;
}

void insere_tipo_variavel_pilha(TipoSimbolo tipo) {
    
    PilhaHash* pilha = pilha_hash;

    if(pilha == NULL) return;

    VariavelSemTipoLst *vst = pilha->variaveis_sem_tipo;

    while(vst != NULL) {
       EntradaHash *resposta = _busca_topo_pilha(vst->chave, pilha);

       if(resposta != NULL) {
           resposta->conteudo.tipo = tipo;
           resposta->conteudo.tamanho = _tamanho(resposta->conteudo.valor_lexico, tipo);
           print_pilha();
       } else {
           //TODO help???
           printf("nao encontrado?? %s\n", vst->chave);
       }
        VariavelSemTipoLst *antigo_vst = vst;
        vst = (VariavelSemTipoLst *)vst->proximo;
        _libera_head_vst(antigo_vst);
    }

    pilha->variaveis_sem_tipo = vst;
    
    return;
}

//TODO fazer as checagens de erros aqui!!!!
void atribuicao_simbolo(EntradaHash *sim1, EntradaHash *sim2) {
    // if(sim1->conteudo.natureza == NATUREZA_VARIAVEL) {
    //     if(sim2->conteudo.natureza == NATUREZA_LITERAL) {

    //     }
    // }
    return;
}

void _atribuicao_simbolo_literal(EntradaHash *sim1, EntradaHash *sim2) {

// TODO mudar tipos dos que pode, erro dos que não pode
//     switch(sim2->conteudo.valor_lexico.tipo_vlex_literal) {
//          case VLEX_LITERAL_BOOL:
//             sim1->conteudo. valor_lexico.valor_bool
//             break;
//          case VLEX_LITERAL_FLOAT:
//             valor_lexico.valor_float=atof(yytext);
//             break;
//          case VLEX_LITERAL_INTEIRO:
//             valor_lexico.valor_int=atoi(yytext);
//             break;
//          case VLEX_LITERAL_CHAR:
//             valor_lexico.valor_char=yytext[1];
//             break;
//          case VLEX_LITERAL_STRING:
//             valor_lexico.valor_string = strdup(yytext+1);
//             valor_lexico.valor_string[strlen(valor_lexico.valor_string)-1] = '\0';
//             break;
//       }

}

ValorLexico copia_vlex(ValorLexico valor_lexico) {

    ValorLexico new_vlex = valor_lexico;
    new_vlex.label = strdup(valor_lexico.label);

    if(tem_valor_string(valor_lexico)) {
        new_vlex.valor_string = strdup(valor_lexico.valor_string);
    }

    return new_vlex;
}

// função que adiciona uma entrada na hash e retorna a recém-adicionada entrada
// chamar ao declarar
// TODO gente como diferenciamos redeclaração e redefinição aqui
/* TODO só procura no topo da pilha, tem que mudar!!!
 -> wait n precisa pq pode declarar algo com msm nome em escopos diferentes 
 -> ta mas e se fizer reatribuição de algo declarado fora do escopo? como achar?
 -> dai tu faz a função de ATRIBUIÇÃO, pq essa aqui tu usa pra DECLARAR.
*/
EntradaHash *_insere_em_pilha(NaturezaSimbolo natureza, TipoSimbolo tipo, ValorLexico valor_lexico) {

    if(pilha_hash == NULL) empilha();

    PilhaHash* pilha = pilha_hash;

    if(pilha == NULL) return NULL;

    char *chave_malloc = _chave(valor_lexico.label, natureza);

    EntradaHash *resposta = _busca_topo_pilha(chave_malloc, pilha);

    if(resposta != NULL) {
        //TODO ve redlecaração ou apenas atribuição??
        //throwDeclaredError(valor_lexico.linha, valor_lexico.label, resposta->conteudo.linha);
        free(chave_malloc);
        return NULL;
    }

    Conteudo conteudo;
    conteudo.linha = valor_lexico.linha;
    conteudo.coluna = -1;
    conteudo.tamanho = _tamanho(valor_lexico, tipo);
    conteudo.tipo = tipo;
    conteudo.natureza = natureza;
    conteudo.argumentos = NULL;
    conteudo.valor_lexico = copia_vlex(valor_lexico);

    resposta = _insere_em_pilha_probing(chave_malloc, pilha, conteudo);

    print_pilha();
    printf("(OUTRA INTERAÇÃO) ____________________________________________________________________________\n");

    return resposta;
}

//TODO checar se já existe
EntradaHash *_insere_em_pilha_probing(char *chave, PilhaHash *pilha, Conteudo conteudo) {

    EntradaHash *tabela = pilha->topo;

    if(tabela == NULL) return NULL;

    int tamanho_tabela = pilha->tamanho_tabela;

    int indice = _indice_hash(chave) % tamanho_tabela;  

    while(tabela != NULL) {

        if(tabela[indice].chave == NULL) {

            tabela[indice].chave = chave;
            tabela[indice].conteudo = conteudo;

            pilha->quantidade_atual++; 

            return &tabela[indice]; 
        }

        indice = _probing(indice, tamanho_tabela);
    }
}

// função que "empilha" uma nova hash em cima da atual
void empilha()
{
    PilhaHash *pilha_aux;
    pilha_aux = (PilhaHash*)malloc(sizeof(PilhaHash));

    pilha_aux->quantidade_atual = 0;
    pilha_aux->tamanho_tabela = TAMANHO_INICIAL_HASH;
    pilha_aux->variaveis_sem_tipo = NULL;

    EntradaHash *tabela = _malloc_tabela();
    pilha_aux->topo = tabela;
    pilha_aux->resto = (struct PilhaHash*)pilha_hash;

    pilha_hash = pilha_aux;
}

//aloca novo array de EntradaHash (com valores NULL pls)
EntradaHash *_malloc_tabela() {

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
        pilha_hash->tamanho_tabela = nova_pilha->tamanho_tabela;
        pilha_hash->quantidade_atual = nova_pilha->quantidade_atual;
        pilha_hash->variaveis_sem_tipo = nova_pilha->variaveis_sem_tipo;
    } else {
        pilha_hash = NULL;
    }

    _libera_tabela(antiga_pilha->topo, antiga_pilha->tamanho_tabela);

    _libera_vsts(antiga_pilha->variaveis_sem_tipo);

    free(antiga_pilha);
}

// função que libera a tabela hash e tudo que há dentro dela (i.e. libera a memória caso necessário)
void _libera_tabela(EntradaHash *tabela, int tamanho_tabela) {

    if(tabela == NULL) return;

    for (int i = 0; i < tamanho_tabela; i++) {

        if(tabela[i].chave != NULL) {

            free(tabela[i].chave);

            // if(tabela[i].conteudo.valor_lexico.label != NULL) {
            //     printf("\nAIAI LABEL %s\n", tabela[i].conteudo.valor_lexico.label);
            // } else {
            //     printf("liberado?");
            // }

            libera_vlex(tabela[i].conteudo.valor_lexico);

            _libera_argumentos(tabela[i].conteudo.argumentos);
        }
    }

    free(tabela);
}

void _libera_argumentos(ArgumentoFuncaoLst *argumento) {

    if(argumento == NULL) return;

    _libera_argumentos((ArgumentoFuncaoLst *)argumento->proximo);

    free(argumento);
}

//TODO lembrar de chamar pf <3
void _libera_head_vst(VariavelSemTipoLst *vst) {

    if(vst == NULL) return;

    if(vst->chave != NULL)
        free(vst->chave);

    free(vst);
}

//TODO lembrar de chamar pf <3
void _libera_vsts(VariavelSemTipoLst *vst) {

    if(vst == NULL) return;

    _libera_vsts((VariavelSemTipoLst *)vst->proximo);

    _libera_head_vst(vst);
}

//#endregion Libera 

//#region Prints
void print_pilha() {

    PilhaHash *aux_pilha = pilha_hash;

    int profundidade = 1;

    while(aux_pilha != NULL) {

        int capacidade = aux_pilha->tamanho_tabela;

        printf("\n\n - ESCOPO Nº%02i DA PILHA - | CAPACIDADE: %i | OCUPAÇÃO: %03i\n", profundidade, capacidade, aux_pilha->quantidade_atual);
        char* str = "-----------------------------------------------------------------------------------------------------------------";
        printf("%s\n", str);
        _print_tabela(aux_pilha->topo, capacidade);
        printf("%s\n\n", str);

        profundidade++;

        aux_pilha = (PilhaHash*)aux_pilha->resto;
    }
}

//printa pilha com tabela e seus valores
void _print_tabela(EntradaHash *tabela, int tamanho) {
    for(int i=0; i < tamanho; i++) {
       EntradaHash entrada = tabela[i];
        
        if(entrada.chave == NULL) continue;

       printf(" ITEM %03i | NATUREZA: %9s | TIPO: %7s | TAMANHO: %4i | RÓTULO: %12s | CHAVE %16s |\n", i+1, _natureza_str(entrada.conteudo.natureza), _tipo_str(entrada.conteudo.tipo), entrada.conteudo.tamanho, entrada.conteudo.valor_lexico.label, entrada.chave);
    }
}

char* _tipo_str(TipoSimbolo tipo) {
    switch(tipo) {
        case TIPO_INT: return "INT"; break;
        case TIPO_FLOAT: return   "FLOAT"; break;
        case TIPO_BOOL: return    "BOOL"; break;
        case TIPO_STRING: return  "STRING"; break;
        case TIPO_CHAR: return    "CHAR"; break;
        case TIPO_OUTRO: return   "OUTRO"; break;
        default: return           "------"; break;
    }
}

char* _natureza_str(NaturezaSimbolo natureza) {
    switch(natureza) {
        case NATUREZA_FUNCAO: return   "FUNCAO"; break;
        case NATUREZA_LITERAL: return  "LITERAL"; break;
        case NATUREZA_VARIAVEL: return "VARIAVEL"; break;
        default: return                "--------"; break;
    }
}
//#endregion Prints