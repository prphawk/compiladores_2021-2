#pragma once
typedef enum tipo_vlex
{
    CARACTERE_ESPECIAL,
    OPERADOR_COMPOSTO,
    IDENTIFICADOR,
    LITERAL,
    OUTRO
} TipoVLex;

typedef enum tipo_vlex_literal
{
    INTEIRO,
    FLOAT,
    BOOL,
    CHAR,
    STRING,
    NAO_LITERAL
} TipoVLexLiteral;

typedef struct valorLexico
{
    int linha;
    TipoVLex tipo_vlex;
    TipoVLexLiteral tipo_vlex_literal;
    char *label;
    union valor {
        int valor_int;
        float valor_float;
        int valor_bool;
        char valor_char;
        char *valor_string;
    } valor;
} valorLexico;

valorLexico atribui_yylval(char* yytext, TipoVLex tipo_vlex, TipoVLexLiteral tipo_vlex_literal, int num_lines);
void libera_valor_lexico(valorLexico valor_lexico);
int tem_valor_string(valorLexico valor_lexico);
