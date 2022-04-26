	.file	"teste.c"
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$5, -8(%rbp)
	movl	$10, -4(%rbp)
	movl	$0, %eax
	popq	%rbp
	ret