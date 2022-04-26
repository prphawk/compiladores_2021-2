	.file	"etapa6.c"
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$4, (%rsp)
	movl	$5, (%rsp)
	movl	(%rsp), %eax
	addq	$4, (%rsp)
	movl	%eax, -8(%rbp)
	subq	$4, (%rsp)
	movl	$10, (%rsp)
	movl	(%rsp), %eax
	addq	$4, (%rsp)
	movl	%eax, -4(%rbp)
	subq	$4, (%rsp)
	movl	$0, (%rsp)
	movl	(%rsp), %eax
	addq	$4, (%rsp)
	movl	%eax, -16(%rbp)
	popq	%rbp
	ret
