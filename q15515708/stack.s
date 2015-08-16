.text
.globl stack_get_rbp
stack_get_rbp:
	movq %rbp, %rax
	ret
