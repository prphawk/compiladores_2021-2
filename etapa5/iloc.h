#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define RFP "rfp"
#define RSP "rsp"
#define RBSS "rbss"
#define RPC "rpc"

typedef enum Operacao
{
    NOP,
    ADD, // ex: add r1, r2 => r3 // r3 = r1 + r2
    SUB, // ex: sub r1, r2 => r3 // r3 = r1 - r2
    MULT, // ex: mult r1, r2 => r3 // r3 = r1 * r2
    DIV, // ex: div r1, r2 => r3 // r3 = r1 / r2
    ADDI, // ex: addI r1, c2 => r3 // r3 = r1 + c2
    SUBI, // ex: subI r1, c2 => r3 // r3 = r1 - c2
    RSUBI, // ex: rsubI r1, c2 => r3 // r3 = c2 - r1
    MULTI, // ex: multI r1, c2 => r3 // r3 = r1 * c2
    DIVI, // ex: divI r1, c2 => r3 // r3 = r1 / c2
    RDIVI, // ex: rdivI r1, c2 => r3 // r3 = c2 / r1
    LSHIFT, // shift left. ex: lshift r1, r2 => r3 // r3 = r1 << r2
    LSHIFTI, // shift left com imediato. ex: lshiftI r1, c2 => r3 // r3 = r1 << c2
    RSHIFT, // shift right. ex: rshift r1, r2 => r3 // r3 = r1 >> r2
    RSHIFTI, // shift right com imediato. ex: rshiftI r1, c2 => r3 // r3 = r1 >> c2
    AND, // ex: and r1, r2 => r3 // r3 = r1 && r2
    ANDI, // and com imediato. ex: andI r1, c2 => r3 // r3 = r1 && c2
    OR, // ex: or r1, r2 => r3 // r3 = r1 || r2
    ORI, // or com imediato. ex: orI r1, c2 => r3 // r3 = r1 || c2
    XOR,  // ex: xor r1, r2 => r3 // r3 = r1 xor r2
    XORI, // xor com imediato. ex: 
    LOAD, // carrega de um valor da memória para registrador. ex: load r1 => r2 // r2 = Memoria(r1)
    LOADAI, // endereçamento imediato. adiciona um valor imediato ao conteúdo do registrador para formar um endereço imediatamente antes de realizar a carga. ex: r1, c2 => r3 // r3 = Memoria(r1 + c2)
    LOADA0, // endereçamento por deslocamento. adicionam o conteúdo de dois registradores para calcular o endereço efetivo imediatamente antes de realizar a carga. ex: loadA0 r1, r2 => r3 // r3 = Memoria(r1 + r2)
    CLOAD, // carrega de um valor da memória para registrador. caractere load. ex: cload r1 => r2
    CLOADAI, // endereçamento imediato. adiciona um valor imediato ao conteúdo do registrador para formar um endereço imediatamente antes de realizar a carga. caractere loadAI (com imediato). ex: cloadAI r1, c2 => r3
    CLOADA0, // endereçamento por deslocamento. adicionam o conteúdo de dois registradores para calcular o endereço efetivo imediatamente antes de realizar a carga. caractere loadA0. ex: cloadA0 r1, r2 => r3
    LOADI, // recebe um inteiro como argumento e coloca este inteiro dentro do registrador alvo. ex: loadI c1 => r2 // r2 = c1
    STORE, // ex: store r1 => r2 // Memoria(r2) = r1
    STOREAI, // ex: storeAI r1 => r2, c3 // Memoria(r2 + c3) = r1
    STOREA0, // ex: storeAO r1 => r2, r3 // Memoria(r2 + r3) = r1
    CSTORE, // caractere store. ex: cstore r1 => r2
    CSTOREAI, // caractere storeAI. ex: cstoreAI r1 => r2, c3
    CSTOREA0, // caractere storeAO. ex: cstoreAO r1 => r2, r3
    I2I, // copia um valor de um registrador para outro, sem conversão. ex: i2i r1 => r2 // r2 = r1 para inteiros
    C2C, // copia um valor de um registrador para outro, sem conversão. ex: c2c r1 => r2 // r2 = r1 para caracteres
    C2I, // realiza conversões considerando a codificação de caracteres ASCII. ex: c2i r1 => r2 // converte um caractere para um inteiro
    I2C, // realiza conversões considerando a codificação de caracteres ASCII. ex: i2c r1 => r2 // converte um inteiro para caractere
    CMP_LT, //less than. ex: cmp_LT r1, r2 -> r3 // r3 = true se r1 < r2, senão r3 = false
    CMP_LE, //less/equal than. ex: cmp_LE r1, r2 -> r3 // r3 = true se r1 <= r2, senão r3 = false
    CMP_EQ, //equal. ex: cmp_EQ r1, r2 -> r3 // r3 = true se r1 = r2, senão r3 = false
    CMP_GE, //greater/equal than. ex: cmp_GE r1, r2 -> r3 // r3 = true se r1 >= r2, senão r3 = false
    CMP_GT, // greater than. ex: cmp_GT r1, r2 -> r3 // r3 = true se r1 > r2, senão r3 = false
    CMP_NE, // not equal. ex: cmp_NE r1, r2 -> r3 // r3 = true se r1 != r2, senão r3 = false
    CBR, // recebe um booleano como argumento e transfere o controle para um de dois rótulos alvo. Os dois rótulos alvo não precisam estar definidos previamente. ex: cbr r1 -> l2, l3 // PC = endereço(l2) se r1 = true, senão PC = endereço(l3)
    JUMPI, // ex: jumpI -> l1 // PC = endereço(l1)
    JUMP  // ex: jump -> r1 // PC = r1
    //TODO halt?
} Operacao;

typedef enum TipoOperando {

    REGISTRADOR, 
    LABEL,
    IMEDIATO,
    REGISTRADOR_ESPECIAL,
    //patchworkTrue,
    //patchworkFalse,

} TipoOperando;

typedef struct OperandoCodigo
{
    char *nome; //TODO ver se precisa?
    TipoOperando tipo;
    int valor;
    struct OperandoCodigo *proximo;
} OperandoCodigo;

typedef struct CodigoILOC
{
    char *label;
    OperandoCodigo *origem;
    Operacao operacao;
    OperandoCodigo *destino;
    struct CodigoILOC *anterior;
} CodigoILOC;

char *gera_nome_rotulo();
char *gera_nome_registrador();
char *gera_nome(int eh_rotulo);
