	.file	"teste.c"
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$5, -4(%rbp)
	movl	-4(%rbp), %eax
	popq	%rbp
	ret