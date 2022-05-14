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
    VLEX_LITERAL_INT,
    VLEX_LITERAL_FLOAT,
    VLEX_LITERAL_BOOL,
    VLEX_LITERAL_CHAR,
    VLEX_LITERAL_STRING,
    VLEX_LITERAL_NULL
} TipoVLexLiteral;

typedef struct valor_lexico
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
void libera_vlex(ValorLexico valor_lexico);
int tem_valor_string(ValorLexico valor_lexico);
int tem_valor_literal_int(ValorLexico valor_lexico);
char* append_str_malloc(char* str1, char* str2);
int compare_eq_str(char* str1, char* str2);
