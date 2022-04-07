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