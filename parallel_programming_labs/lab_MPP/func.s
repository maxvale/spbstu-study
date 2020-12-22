	.file	"func_vec.c"
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"This arrays are in superposition "
	.align 8
.LC1:
	.string	"This arrays are not in superposition "
	.text
	.p2align 4
	.globl	compareArraysVec
	.type	compareArraysVec, @function
compareArraysVec:
.LFB4230:
	.cfi_startproc
	endbr64
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movq	%rdx, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	movq	%rdi, %rbp
	movslq	%ecx, %rdi
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movq	%rdi, %r13
	salq	$2, %rdi
	movq	%rsi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	call	malloc@PLT
	testl	%r13d, %r13d
	jle	.L2
	leal	-1(%r13), %edx
	xorl	%esi, %esi
	movl	$1, %edi
	movq	0(%rbp,%rdx,4), %xmm3
	movq	(%rbx,%rdx,4), %xmm2
	.p2align 4,,10
	.p2align 3
.L3:
	movq	(%r12,%rsi,4), %xmm1
	movdqa	%xmm3, %xmm0
	pcmpeqd	%xmm1, %xmm0
	pcmpeqd	%xmm2, %xmm1
	por	%xmm1, %xmm0
	movq	%xmm0, %rcx
	movq	%xmm0, (%rax,%rsi,4)
	imull	%ecx, %edi
	movq	%rsi, %rcx
	addq	$1, %rsi
	cmpq	%rcx, %rdx
	jne	.L3
	testl	%edi, %edi
	jne	.L2
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	leaq	.LC0(%rip), %rdi
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	puts@PLT
.L2:
	.cfi_restore_state
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	leaq	.LC1(%rip), %rdi
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	puts@PLT
	.cfi_endproc
.LFE4230:
	.size	compareArraysVec, .-compareArraysVec
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
