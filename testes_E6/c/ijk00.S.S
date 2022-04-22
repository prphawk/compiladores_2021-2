# 1 "../testes_E6/c/ijk00.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "../testes_E6/c/ijk00.S"
 .file "ijk00.c"
 .text
 .comm z,4,4
 .globl main
 .type main, @function
main:
.LFB0:
 .cfi_startproc
 endbr64
 pushq %rbp
 .cfi_def_cfa_offset 16
 .cfi_offset 6, -16
 movq %rsp, %rbp
 .cfi_def_cfa_register 6
 movl $923, z(%rip)
 movl $456, -8(%rbp)
 movl z(%rip), %edx
 movl -8(%rbp), %eax
 addl %edx, %eax
 movl %eax, -4(%rbp)
 movl -4(%rbp), %eax
 popq %rbp
 .cfi_def_cfa 7, 8
 ret
 .cfi_endproc
.LFE0:
 .size main, .-main
 .ident "GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
 .section .note.GNU-stack,"",@progbits
 .section .note.gnu.property,"a"
 .align 8
 .long 1f - 0f
 .long 4f - 1f
 .long 5
0:
 .string "GNU"
1:
 .align 8
 .long 0xc0000002
 .long 3f - 2f
2:
 .long 0x3
3:
 .align 8
4:
