typedef enum tipo
{
    CARACTERE_ESPECIAL,
    OPERADOR_COMPOSTO,
    IDENTIFICADOR,
    LITERAL
} Tipo;

typedef enum tipoLiteral
{
    INTEIRO,
    FLOAT,
    CHAR,
    STRING,
    NAO_LITERAL
} TipoLiteral;

typedef struct valorLexico
{
    int linha;
    Tipo tipo;
    TipoLiteral tipo_literal;
    union valor {
        int valor_int;
        float valor_float;
        char valor_char;
        char *valor_string;
    } valor;
} valorLexico;