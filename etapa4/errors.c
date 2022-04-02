#include "errors.h"
#include "tabela_simbolos.h"


/*TODO Mensagens de erro significativas devem ser fornecidas.
Elas devem descrever em linguagem natural o erro semantico, as linhas envolvidas, os identificadores e a natureza destes.*/
void throwUndeclaredError(int linha, char *input)
{
  printf("ERRO linha %i: Variável %s não declarada.\n", linha, input);
  exit(ERR_UNDECLARED);
}

void throwDeclaredError(int linha, char *input, int linha_declaracao)
{
  printf("ERRO linha %i: Variável já declarada na linha %i.\n", linha, linha_declaracao);
  exit(ERR_DECLARED);
}

void throwVariableError(int linha, char *input, int linha_declaracao)
{
  printf("ERRO linha %i: Identificador \"%s\" de natureza VARIÁVEL declarada em linha %i sendo utilizada de forma inválida.\n", linha, input, linha_declaracao);
  exit(ERR_VARIABLE);
}

void throwVectorError(int linha, char *input, int linha_declaracao)
{
  printf("ERRO linha %i: Identificador \"%s\" de natureza VETOR declarada em linha %i sendo utilizada de forma inválida.\n", linha, input, linha_declaracao);
  exit(ERR_VECTOR);
}

void throwFunctionError(int linha, char *input, int linha_declaracao)
{
  printf("ERRO linha %i: Identificador \"%s\" de natureza FUNÇÃO declarada em linha %i sendo utilizada de forma inválida.\n", linha, input, linha_declaracao);
  exit(ERR_FUNCTION);
}
void throwWrongTypeError(int linha, char *input)
{
  printf("ERRO linha %i", linha);
  exit(ERR_WRONG_TYPE);
}

void throwStringToXError(int linha, char *input)
{
  printf("ERRO linha %i", linha);
  exit(ERR_STRING_TO_X);
}

void throwCharToXError(int linha, char *input)
{
  printf("ERRO linha %i", linha);

  exit(ERR_CHAR_TO_X);
}

void throwStringSizeError(int linha, char *input, int linha_declaracao)
{
  printf("ERRO linha %i", linha);
  exit(ERR_STRING_MAX);
}

void throwStringVectorError(int linha)
{
  printf("ERRO linha %i", linha);
  exit(ERR_STRING_VECTOR);
}

void throwExcessArgsError(int linha, char *input, int linha_declaracao)
{
  printf("ERRO linha %i", linha);
  exit(ERR_EXCESS_ARGS);
}

void throwMissingArgsError(int linha, char *input, int linha_declaracao)
{
  printf("ERRO linha %i", linha);
  exit(ERR_MISSING_ARGS);
}

void throwWrongTypeArgsError(int linha, char *input, int linha_declaracao)
{
  printf("ERRO linha %i", linha);
  exit(ERR_WRONG_TYPE_ARGS);
}

void throwFunctionStringError(int linha, char *input, int linha_declaracao)
{
  printf("ERRO linha %i", linha);
  exit(ERR_WRONG_TYPE_ARGS);
}

void throwWrongParInput(int linha)
{
  printf("ERRO linha %i", linha);
  exit(ERR_WRONG_PAR_INPUT);
}

void throwWrongParOutput(int linha)
{
  printf("ERRO linha %i", linha);
  exit(ERR_WRONG_PAR_OUTPUT);
}

void throwShiftError(int linha)
{
  printf("ERRO linha %i", linha);
  exit(ERR_WRONG_PAR_SHIFT);
}

void throwReturnError(int linha)
{
  printf("ERRO linha %i", linha);
  exit(ERR_WRONG_PAR_RETURN);
}