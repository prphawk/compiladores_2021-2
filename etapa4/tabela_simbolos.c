#include "tabela_simbolos.h"

// função que recebe um nome (label?) e uma natureza (variável, função, literal) e cria uma chave dando append nos dois
// exemplo out: nomevar1
char *chave(char *nome, NaturezaSimbolo natureza)
{
    char str[2];
    sprintf(str, "%d", natureza);
    return append_str_malloc(nome, str);
    //free dps!!!
}

// função que retorna um valor hash a partir da chave
unsigned long indiceHash(char *chave)
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

// TODO função que adiciona uma entrada na hash
// retorna a recém-adicionada entrada
EntradaHash *adicionaHash(NaturezaSimbolo natureza, TipoSimbolo tipo, ValorLexico valor_lexico)
{
    // usa o indiceHash() aqui dentro
    return NULL;
}

// TODO função que retorna uma entrada específica da hash a partir de sua chave
EntradaHash *encontraNaPilha(char *chave, PilhaHash *pilha)
{
    if(pilha == NULL) return NULL;
    
    EntradaHash *resposta = encontraNaTabela(chave, pilha->topo, pilha->capacidade);

    if(resposta != NULL) return resposta;
    
    PilhaHash *resto = (PilhaHash*)pilha->resto;
	
    return encontraNaPilha(chave, resto);        
}

EntradaHash *encontraNaTabela(char *chave, EntradaHash **tabela, int capacidade_hash) {

    int indice = indiceHash(chave) % capacidade_hash;  

    while(tabela != NULL) {

        if(tabela[indice]->chave == chave)
            return tabela[indice]; 
            
        indice = probing(indice, capacidade_hash);
    }

    return NULL;
}

int probing(int indice, int capacidade_hash) {

    indice++;
		
    indice %= capacidade_hash;

    return indice;
}

// TODO expandir tamanho da tabela quando atinge 75% da capacidade
PilhaHash *expandeHash(PilhaHash *pilha)
{
   return NULL;        
}

// TODO função que adiciona um argumento à lista de argumentos de uma variável > DO TIPO FUNÇÃO < (checar!!!)
void adicionaArgumento(EntradaHash entrada, TipoSimbolo tipo, int tamanho, ValorLexico valor_lexico)
{
    // não precisa usar o "chave", guarda somente o >NOME< do argumento em argumento.nome
    // cria um entradaArgumento
    return;
}

// TODO função que "empilha" uma nova hash em cima da atual
void empilhaHash()
{
    return;
}

// TODO função que "desempilha" (elimina) a hash no topo da pilha
void desempilhaHash()
{
    return;
}

// TODO função que libera a pilha de hash e tudo que há dentro dela (i.e. libera a memória caso necessário)
void liberaPilhaHash()
{
    return;
}