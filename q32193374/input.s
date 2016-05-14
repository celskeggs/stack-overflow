	.file	"input.c"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB0:
	.text
.LHOTB0:
	.globl	wait
	.type	wait, @function
wait:
.LFB0:
	.cfi_startproc
	subq	$88, %rsp
	.cfi_def_cfa_offset 96
	movl	$21505, %esi
	xorl	%edi, %edi
	leaq	8(%rsp), %rdx
	xorl	%eax, %eax
	call	ioctl
	leaq	44(%rsp), %rdx
	movl	$21505, %esi
	xorl	%edi, %edi
	xorl	%eax, %eax
	call	ioctl
	movl	52(%rsp), %eax
	leaq	44(%rsp), %rdx
	movl	$21506, %esi
	xorl	%edi, %edi
	andl	$-1516, 44(%rsp)
	andl	$-2, 48(%rsp)
	andl	$-305, %eax
	andl	$-32844, 56(%rsp)
	orl	$48, %eax
	movl	%eax, 52(%rsp)
	xorl	%eax, %eax
	call	ioctl
	leaq	7(%rsp), %rsi
	movl	$1, %edx
	xorl	%edi, %edi
	call	read
	leaq	8(%rsp), %rdx
	movl	$21506, %esi
	xorl	%edi, %edi
	xorl	%eax, %eax
	call	ioctl
	addq	$88, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE0:
	.size	wait, .-wait
	.section	.text.unlikely
.LCOLDE0:
	.text
.LHOTE0:
	.ident	"GCC: (GNU) 5.2.0"
	.section	.note.GNU-stack,"",@progbits
