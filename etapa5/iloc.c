#include "iloc.h"

int global_num_rotulos = 1;
int global_num_registradores = 1;

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
        n = global_num_rotulos;
        global_num_rotulos++;
        c = 'L';
    }
    else
    {
        n = global_num_registradores;
        global_num_registradores++;
        c = 'r';
    }
    char *numero = calloc(sizeof(char), 10); // +1 for null
    snprintf(numero, 11, "%c%d", c, n);
    return numero;
}