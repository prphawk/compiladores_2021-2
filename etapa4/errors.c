#include "errors.h"
#include "tabela_simbolos.h"


/*TODO Mensagens de erro significativas devem ser fornecidas.
Elas devem descrever em linguagem natural o erro semantico, as linhas envolvidas, os identificadores e a natureza destes.*/
void throwUndeclaredError(int linha, char *input)
{
  printf("[ERR_UNDECLARED] linha %i: Identificador %s não declarado.\n", linha, input);
  exit(ERR_UNDECLARED);
}

void throwDeclaredError(int linha, char *input, int linha_declaracao)
{
  printf("[ERR_DECLARED] linha %i: Identificador já declarado na linha %i.\n", linha, linha_declaracao);
  exit(ERR_DECLARED);
}

void throwVariableError(int linha, char *input, int linha_declaracao)
{
  printf("[ERR_VARIABLE] linha %i: Identificador \"%s\" de natureza VARIÁVEL declarada em linha %i sendo utilizada de forma inválida.\n", linha, input, linha_declaracao);
  exit(ERR_VARIABLE);
}

void throwVectorError(int linha, char *input, int linha_declaracao)
{
  printf("[ERR_VECTOR] linha %i: Identificador \"%s\" de natureza VETOR declarada em linha %i sendo utilizada de forma inválida.\n", linha, input, linha_declaracao);
  exit(ERR_VECTOR);
}

void throwFunctionError(int linha, char *input, int linha_declaracao)
{
  printf("[ERR_FUNCTION] linha %i: Identificador \"%s\" de natureza FUNÇÃO declarada em linha %i sendo utilizada de forma inválida.\n", linha, input, linha_declaracao);
  exit(ERR_FUNCTION);
}
void throwWrongTypeError(int linha, char *input, char *x)
{
  printf("[ERR_WRONG_TYPE] linha %i: Tipo de \"%s\" sendo ilegalmente atribuído à variável \"%s\"\n", linha, input, x);
  exit(ERR_WRONG_TYPE);
}

void throwStringToXError(int linha, char *input, char *x)
{
  printf("[ERR_STRING_TO_X] linha %i: Tipo STRING em \"%s\" sendo convertido ilegalmente para \"%s\"\n", linha, input, x);
  exit(ERR_STRING_TO_X);
}

void throwCharToXError(int linha, char *input, char *x)
{
  printf("[ERR_CHAR_TO_X] linha %i: Tipo CHAR em \"%s\" sendo convertido ilegalmente para \"%s\"\n", linha, input, x);

  exit(ERR_CHAR_TO_X);
}

void throwStringSizeError(int linha, char *input, int linha_declaracao)
{
  printf("[ERR_STRING_MAX] linha %i: \"%s\" excede o tamanho máximo declarado em linha %i\n", linha, input, linha_declaracao);
  exit(ERR_STRING_MAX);
}

void throwStringVectorError(int linha, char *input)
{
  printf("[ERR_STRING_VECTOR] linha %i: Vetor \"%s\" não pode ser do tipo STRING.\n", linha, input);
  exit(ERR_STRING_VECTOR);
}

void throwExcessArgsError(int linha, char *input, int linha_declaracao)
{
  printf("[ERR_EXCESS_ARGS] linha %i: Função \"%s\" declarada em linha %i sendo chamada com argumentos a mais.\n", linha, input, linha_declaracao);
  exit(ERR_EXCESS_ARGS);
}

void throwMissingArgsError(int linha, char *input, int linha_declaracao)
{
  printf("[ERR_MISSING_ARGS] linha %i: Função \"%s\" declarada em linha %i sendo chamada com argumentos a menos.\n", linha, input, linha_declaracao);
  exit(ERR_MISSING_ARGS);
}

void throwWrongTypeArgsError(int linha, char *input, int linha_declaracao)
{
  printf("[ERR_WRONG_TYPE_ARGS] linha %i: Argumento \"%s\" possui tipo incompatível com função declarada em linha %i.\n", linha, input, linha_declaracao);
  exit(ERR_WRONG_TYPE_ARGS);
}

void throwFunctionStringError(int linha, char *input)
{
  printf("[ERR_WRONG_TYPE_ARGS] linha %i em %s: Retorno, argumentos e parâmetros de funções não podem ser do tipo STRING.\n", linha, input);
  exit(ERR_WRONG_TYPE_ARGS);
}

void throwWrongParInput(int linha, char *input)
{
  printf("[ERR_WRONG_PAR_INPUT] linha %i: Operando \"%s\" em INPUT não é do tipo FLOAT ou INT.\n", linha, input);
  exit(ERR_WRONG_PAR_INPUT);
}

void throwWrongParOutput(int linha, char *input)
{
  printf("[ERR_WRONG_PAR_OUTPUT] linha %i: Operando \"%s\" em OUTPUT não é do tipo FLOAT ou INT.\n", linha, input);
  exit(ERR_WRONG_PAR_OUTPUT);
}

void throwWrongParShift(int linha)
{
  printf("[ERR_WRONG_PAR_SHIFT] linha %i: Operador inteiro à direita de comando SHIFT é maior que 16.\n", linha);
  exit(ERR_WRONG_PAR_SHIFT);
}

void throwReturnError(int linha, char *input)
{
  printf("[ERR_WRONG_PAR_RETURN] linha %i: Tipo de \"%s\" é retorno incompatível com função.\n", linha, input);
  exit(ERR_WRONG_PAR_RETURN);
}