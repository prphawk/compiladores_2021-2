#pragma once
typedef enum tipo
{
    CARACTERE_ESPECIAL,
    OPERADOR_COMPOSTO,
    IDENTIFICADOR,
    LITERAL,
    OUTRO
} Tipo;

typedef enum tipoLiteral
{
    INTEIRO,
    FLOAT,
    BOOL,
    CHAR,
    STRING,
    NAO_LITERAL
} TipoLiteral;

typedef struct valorLexico
{
    int linha;
    Tipo tipo;
    TipoLiteral tipo_literal;
    char *label;
    union valor {
        int valor_int;
        float valor_float;
        int valor_bool;
        char valor_char;
        char *valor_string;
    } valor;
} valorLexico;

valorLexico atribui_yylval(char* yytext, Tipo tipo, TipoLiteral tipo_literal, int num_lines);
void libera_valor_lexico(valorLexico valor_lexico);
