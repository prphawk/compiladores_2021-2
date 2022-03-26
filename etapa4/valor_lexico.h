#pragma once
typedef enum tipo_vlex
{
    VLEX_TIPO_CARACTERE_ESPECIAL,
    VLEX_TIPO_OPERADOR_COMPOSTO,
    VLEX_TIPO_IDENTIFICADOR,
    VLEX_TIPO_LITERAL,
    VLEX_TIPO_OUTRO
} TipoVLex;

typedef enum tipo_vlex_literal
{
    VLEX_LITERAL_INTEIRO,
    VLEX_LITERAL_FLOAT,
    VLEX_LITERAL_BOOL,
    VLEX_LITERAL_CHAR,
    VLEX_LITERAL_STRING,
    VLEX_LITERAL_NULL
} TipoVLexLiteral;

typedef struct valorLexico
{
    int linha;
    TipoVLex tipo_vlex;
    TipoVLexLiteral tipo_vlex_literal;
    char *label;
    int valor_int;
    float valor_float;
    int valor_bool;
    char valor_char;
    char *valor_string;
} ValorLexico;

ValorLexico atribui_yylval(char* yytext, TipoVLex tipo_vlex, TipoVLexLiteral tipo_vlex_literal, int num_lines);
void libera_valor_lexico(ValorLexico valor_lexico);
int tem_valor_string(ValorLexico valor_lexico);
