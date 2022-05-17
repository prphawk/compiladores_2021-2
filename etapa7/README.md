## Aline Hommerding Amorim (00301570) & Mayra Camargo Cademartori (00301639)

Em etapa7, para compilar os exemplos:

```
make
./etapa7 < ex1.txt > saida1
./etapa7 < ex2.txt > saida2
./etapa7 -o < ex1.txt > saida1_otim
./etapa7 -o < ex2.txt > saida2_otim
```

Não foi entregue a etapa 6, a saída é ILOC.

## Exemplo 1

-   Arquivo do exemplo: `ex1.txt`

|    Arquivo    | Otimizado | Temporários usados | Linhas de código |
| :-----------: | :-------: | :----------------: | :--------------: |
|   `saida1`    |    não    |         41         |        83        |
| `saida1_otim` |    sim    |         26         |        57        |

## Exemplo 2

-   Arquivo do exemplo: `ex2.txt`

|    Arquivo    | Otimizado | Temporários usados | Linhas de código |
| :-----------: | :-------: | :----------------: | :--------------: |
|   `saida2`    |    não    |         33         |        69        |
| `saida2_otim` |    sim    |         22         |        47        |

## Melhorias

### Compressão de instruções NOP com label:

`ex1.txt`

-   Código antigo:

```
L5: nop
loadAI rfp, 12 => r9
```

-   Código otimizado:

```
L5: loadAI rfp, 12 => r9
```

Observações: O código abaixo de NOP não tem rótulo próprio.

### Remoção de código inalcançável

`ex2.txt`

-   Código antigo:

```
jump  => r19
loadAI rfp, 0 => r23
loadAI rfp, 4 => rsp
loadAI rfp, 8 => rfp
jump  => r23
```

-   Código otimizado:

```
jump  => r19
```

Observações: Os códigos removidos não têm rótulo próprio.

### Propagação de cópia de imediatos

`ex2.txt`

-   Código antigo:

```
loadI 0 => r4
storeAI r4 => rfp, 4
... (continuação)
loadI 0 => r10
cmp_GT r9, r10 => r11
loadI 0 => r12
cmp_NE r11, r12 => r13
```

-   Código otimizado:

```
loadI 0 => r4
storeAI r4 => rfp, 4
... (continuação)
cmp_GT r9, r4 => r11
cmp_NE r11, r4 => r13
```

Observações: Os temporários de imediatos não sofreram alterações ao longo do fluxo do programa.

### Substituição de temporários de imediatos por constantes em expressões algébricas + Simplificação de expressões algébricas

`ex2.txt`

-   Código antigo:

```
loadI 1 => r34
add r31, r34 => r35
```

-   Código otimizado:

```
 addI r31, 1 => r35
```

Observações: Os temporários de imediatos não sofreram alterações ao longo do fluxo do programa.

### Remoção de operações identidade em expressões algébricas

`ex1.txt`

-   Código antigo:

```
 multI r45, 1 => r47
loadI 4 => r48
multI r47, 4 => r49
storeAI r49 => rfp, 16
loadAI rfp, 4 => r51
addI r51, 0 => r53
storeAI r53 => rfp, 16
loadAI rfp, 8 => r55
subI r55, 0 => r57
storeAI r57 => rfp, 16
loadAI rfp, 12 => r59
divI r59, 1 => r61
storeAI r61 => rfp, 16
```

-   Código otimizado:

```
loadI 4 => r48
multI r45, 4 => r49
storeAI r49 => rfp, 16
loadAI rfp, 4 => r51
storeAI r51 => rfp, 16
loadAI rfp, 8 => r55
storeAI r55 => rfp, 16
loadAI rfp, 12 => r59
storeAI r59 => rfp, 16
```

### Remoção de instruções redundantes

`ex1.txt`

-   Código antigo:

```
storeAI r59 => rfp, 16
loadAI rfp, 16 => r62
storeAI r62 => rfp, 12
```

-   Código otimizado:

```
storeAI r59 => rfp, 16
storeAI r59 => rfp, 12
```

`ex2.txt`

-   Código antigo:

```
loadI 6 => r27
storeAI r27 => rfp, 0
storeAI rsp => rsp, 4
storeAI rfp => rsp, 8
loadAI rfp, 0 => r29
storeAI r29 => rsp, 12
```

-   Código otimizado:

```
loadI 6 => r27
storeAI r27 => rfp, 0
storeAI rsp => rsp, 4
storeAI rfp => rsp, 8
storeAI r27 => rsp, 12
```

Observações: os temporarios substituídos não são reutilizados, o substituidor não tem seu valor mudado durante o fluxo do programa.
(É feito a checagem de mudanças de valores, desvios e entradas de desvios.)

### Simplificação de código de retorno

`ex2.txt`

-   Código antigo:

```
loadAI rfp, 4 => r8
loadAI rfp, 8 => r9
i2i r8 => rsp
i2i r9 => rfp
```

-   Código otimizado:

```
loadAI rfp, 4 => rsp
loadAI rfp, 8 => rfp
```

## Código final de `ex1.txt` (otimizado)

```
loadI 58 => rbss
jumpI  => L8
L8: addI rsp, 52 => rsp
loadI 1 => r2
storeAI r2 => rfp, 0
loadI 0 => r4
storeAI r4 => rfp, 4
loadI 2 => r6
storeAI r6 => rfp, 8
loadI 3 => r8
storeAI r8 => rfp, 12
L5: loadAI rfp, 12 => r9
cmp_GT r9, r4 => r11
cmp_NE r11, r4 => r13
cbr r13 => L6, L7
L6: loadAI rfp, 0 => r14
loadAI rfp, 4 => r15
cmp_LT r14, r15 => r16
cmp_NE r16, r4 => r18
cbr r18 => L1, L4
L1: loadAI rfp, 8 => r19
loadAI rfp, 12 => r20
cmp_LT r19, r20 => r21
cmp_NE r21, r4 => r23
cbr r23 => L3, L4
L3: loadAI rfp, 0 => r25
loadAI rfp, 4 => r26
add r25, r26 => r27
storeAI r27 => rfp, 16
loadAI rfp, 8 => r29
addI r29, 2 => r31
storeAI r31 => rfp, 8
jumpI  => L2
L4: loadAI rfp, 8 => r33
loadAI rfp, 12 => r34
add r33, r34 => r35
storeAI r35 => rfp, 16
loadAI rfp, 0 => r37
addI r37, 1 => r39
storeAI r39 => rfp, 0
jumpI  => L2
L2: loadAI rfp, 12 => r41
subI r41, 1 => r43
storeAI r43 => rfp, 12
jumpI  => L5
L7: loadAI rfp, 0 => r45
loadI 4 => r48
multI r45, 4 => r49
storeAI r49 => rfp, 16
loadAI rfp, 4 => r51
storeAI r51 => rfp, 16
loadAI rfp, 8 => r55
storeAI r55 => rfp, 16
loadAI rfp, 12 => r59
storeAI r59 => rfp, 16
storeAI r59 => rfp, 12
halt
```

## Código final de `ex2.txt` (otimizado)

```
loadI 48 => rbss
jumpI  => L4
L3: i2i rsp => rfp
addI rsp, 32 => rsp
loadAI rfp, 12 => r22
storeAI r22 => rfp, 20
loadI 1 => r2
cmp_EQ r22, r2 => r3
loadI 0 => r4
cmp_NE r3, r4 => r5
cbr r5 => L2, L1
L2: loadAI rfp, 20 => r6
storeAI r6 => rfp, 16
loadAI rfp, 0 => r7
loadAI rfp, 4 => rsp
loadAI rfp, 8 => rfp
jump  => r7
L1: storeAI rsp => rsp, 4
storeAI rfp => rsp, 8
loadAI rfp, 20 => r11
subI r11, 1 => r13
storeAI r13 => rsp, 12
addI rpc, 3 => r14
storeAI r14 => rsp, 0
jumpI  => L3
ret: loadAI rsp, 16 => r15
loadAI rfp, 20 => r16
mult r15, r16 => r17
storeAI r17 => rfp, 24
storeAI r17 => rfp, 16
loadAI rfp, 0 => r19
loadAI rfp, 4 => rsp
loadAI rfp, 8 => rfp
jump  => r19
L4: addI rsp, 12 => rsp
loadI 6 => r27
storeAI r27 => rfp, 0
storeAI rsp => rsp, 4
storeAI rfp => rsp, 8
storeAI r27 => rsp, 12
addI rpc, 3 => r30
storeAI r30 => rsp, 0
jumpI  => L3
ret: loadAI rsp, 16 => r31
storeAI r31 => rbss, 0
storeAI r4 => rfp, 12
halt
```
