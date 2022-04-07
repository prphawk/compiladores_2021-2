#pragma once

#include <string.h>

#define RFP "rfp"
#define RSP "rsp"
#define RBSS "rbss"
#define RPC "rpc"

int num_rotulos = 1;
int num_registradores = 1;

typedef enum Operacao
{
    NOP,
    ADD,
    SUB,
    MULT,
    DIV,
    ADDI,
    SUBI,
    RSUBI,
    MULTI,
    DIVI,
    RDIVI,
    LSHIFT,
    LSHIFTI,
    RSHIFT,
    RSHIFTI,
    AND,
    ANDI,
    OR,
    ORI,
    XOR,
    XORI,
    LOAD,
    LOADAI,
    LOADA0,
    CLOAD,
    CLOADAI,
    CLOADA0,
    LOADI,
    STORE,
    STOREAI,
    STOREA0,
    CSTORE,
    CSTOREAI,
    CSTOREA0,
    I2I,
    C2C,
    C2I,
    I2C,
    CMP_LT,
    CMP_LE,
    CMP_EQ,
    CMP_GE,
    CMP_GT,
    CMP_NE,
    CBR,
    JUMPI,
    JUMP
} Operacao;

typedef struct CodigoILOC
{
    char *label;
    char *origem1;
    char *origem2;
    Operacao operacao;
    char *destino1;
    char *destino2;
    CodigoILOC *anterior;
} CodigoILOC;