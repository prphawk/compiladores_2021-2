#include <stdio.h>
#include <stdlib.h>
#include "tabela_simbolos.h"

#define ERR_UNDECLARED       10
#define ERR_DECLARED         11
#define ERR_VARIABLE         20
#define ERR_VECTOR           21
#define ERR_FUNCTION         22
#define ERR_WRONG_TYPE       30
#define ERR_STRING_TO_X      31
#define ERR_CHAR_TO_X        32
#define ERR_STRING_MAX       33
#define ERR_STRING_VECTOR    34
#define ERR_MISSING_ARGS     40
#define ERR_EXCESS_ARGS      41
#define ERR_WRONG_TYPE_ARGS  42
#define ERR_FUNCTION_STRING  43
#define ERR_WRONG_PAR_INPUT  50
#define ERR_WRONG_PAR_OUTPUT 51
#define ERR_WRONG_PAR_RETURN 52
#define ERR_WRONG_PAR_SHIFT  53

void throwUndeclaredError(int linha, char *input);
void throwDeclaredError(int linha, char *input, int previousDeclarationLine);
void throwVariableError(int linha, char *input, int linha_declaracao_input);
void throwVectorError(int linha, char *input, int linha_declaracao_input);
void throwFunctionError(int linha, char *input, int linha_declaracao_input);
void throwWrongTypeError(int linha, char *input);
void throwStringToXError(int linha, char *input, char *x);
void throwCharToXError(int linha, char *input, char *x);
void throwStringSizeError(int linha, char *input, int linha_declaracao_input);
void throwStringVectorError(int linha, char *input);
void throwExcessArgsError(int linha, char *input, int linha_declaracao_input);
void throwMissingArgsError(int linha, char *input, int linha_declaracao_input);
void throwWrongTypeArgsError(int linha, char *input, int linha_declaracao_input);
void throwFunctionStringError(int linha, char *input);
void throwWrongParInput(int linha, char *input);
void throwWrongParOutput(int linha, char *input);
void throwWrongParShift(int linha);
void throwReturnError(int linha, char *input);
