#include "iloc.h"

char *gera_nome_rotulo()
{
    return gera_nome(1);
}

char *gera_nome_registrador()
{
    return gera_nome(0);
}

char *gera_nome(int eh_rotulo)
{
    int n;
    char c;
    if (eh_rotulo == 0)
    {
        n = num_rotulos;
        num_rotulos++;
        c = "L";
    }
    else
    {
        n = num_registradores;
        num_registradores++;
        c = "r";
    }
    char *numero = calloc(sizeof(char), 10); // +1 for null
    snprintf(numero, 11, "%c%d", c, n);
    return numero;
}

CodigoILOC cria_codigo(char *origem1, char *origem2, Operacao operacao, char *destino1, char *destino2)
{
    CodigoILOC *codigo = malloc(sizeof(CodigoILOC));
    codigo->origem1 = origem1 ? origem1 : NULL;
    codigo->origem2 = origem2 ? origem2 : NULL;
    codigo->operacao = operacao;
    codigo->destino1 = destino1 ? destino1 : NULL;
    codigo->destino2 = destino2 ? destino2 : NULL;
}