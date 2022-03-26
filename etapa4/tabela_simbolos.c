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

// TODO função que retorna um valor hash a partir da chave
int indiceHash(char *chave)
{
    // usa o chave() aqui dentro -> what? a entrada eh a chave já

    //https://stackoverflow.com/questions/7666509/hash-function-for-string
    unsigned long hash = 5381;
    int c;

    while (c = *chave++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % TAMANHO_HASH;
}

// TODO função que adiciona uma entrada na hash
// retorna a recém-adicionada entrada
EntradaHashSimbolo *adicionaHash(NaturezaSimbolo natureza, TipoSimbolo tipo, int tamanho, ValorLexico valor_lexico)
{
    // usa o indiceHash() aqui dentro
    return NULL;
}

// TODO função que retorna uma entrada específica da hash a partir de sua chave
EntradaHashSimbolo *entradaHash(char *chave)
{
   int indice = indiceHash(chave);  
	
   //move in array until an empty 
//    while(hashArray[hashIndex] != NULL) {
	
//       if(hashArray[hashIndex]->key == key)
//          return hashArray[hashIndex]; 
			
//       //go to next cell
//       ++hashIndex;
		
//       //wrap around the table
//       hashIndex %= SIZE;
//    }        
	
   return NULL;        
}

// TODO função que adiciona um argumento à lista de argumentos de uma variável > DO TIPO FUNÇÃO < (checar!!!)
void adicionaArgumento(EntradaHashSimbolo entrada, TipoSimbolo tipo, int tamanho, ValorLexico valor_lexico)
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