#include "tabela_simbolos.h"

// TODO função que recebe um nome (label?) e uma natureza (variável, função, literal) e cria uma chave dando append nos dois
char *chave(char *nome, Natureza natureza)
{
    return "oi";
}

// TODO função que retorna um valor hash a partir da chave
int indiceHash(char *chave)
{
    // usa o chave() aqui dentro
    return 0;
}

// TODO função que adiciona uma entrada na hash
// retorna a recém-adicionada entrada
entradaHashSimbolo *adicionaHash(Natureza natureza, TipoSimbolo tipo, int tamanho, valorLexico valor_lexico)
{
    // usa o indiceHash() aqui dentro
    return NULL;
}

// TODO função que retorna uma entrada específica da hash a partir de sua chave
entradaHashSimbolo *entradaHash(char *nome, Natureza natureza)
{
    return NULL;
}

// TODO função que adiciona um argumento à lista de argumentos de uma variável > DO TIPO FUNÇÃO < (checar!!!)
void adicionaArgumento(entradaHashSimbolo entrada, TipoSimbolo tipo, int tamanho, valorLexico valor_lexico)
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