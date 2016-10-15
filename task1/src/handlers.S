.extern c_handler

#define EXCEPTION(no) \
__entry##no:           \
	pushq $(no);     \
	jmp __common


#define INTERRUPT(no) \
__entry##no:          \
	pushq $(0);     \
	pushq $(no);    \
	jmp __common


__common:
	
	push %rax
	push %rbx
	push %rcx
	push %rdx
	push %rbp
	push %rdi
	push %rsi
	push %r9
	push %r15  

	movq %rsp, %rdi
	addq $72, %rdi
	call c_handler
	
	pop %r15
	pop %r9
	pop %rsi
	pop %rdi
	pop %rbp
	pop %rdx
	pop %rcx
	pop %rbx
	pop %rax

	addq $16, %rsp
	iretq	

INTERRUPT(0)
INTERRUPT(1)
INTERRUPT(2)
INTERRUPT(3)
INTERRUPT(4)
INTERRUPT(5)
INTERRUPT(6)
INTERRUPT(7)
EXCEPTION(8)
INTERRUPT(9)
EXCEPTION(10)
EXCEPTION(11)
EXCEPTION(12)
EXCEPTION(13)
EXCEPTION(14)
INTERRUPT(15)
INTERRUPT(16)
EXCEPTION(17)
INTERRUPT(18)
INTERRUPT(19)
INTERRUPT(20)
INTERRUPT(21)
INTERRUPT(22)
INTERRUPT(23)
INTERRUPT(24)
INTERRUPT(25)
INTERRUPT(26)
INTERRUPT(27)
INTERRUPT(28)
INTERRUPT(29)
INTERRUPT(30)
INTERRUPT(31)
INTERRUPT(32)
INTERRUPT(33)
INTERRUPT(34)
INTERRUPT(35)
INTERRUPT(36)
INTERRUPT(37)
INTERRUPT(38)
INTERRUPT(39)
INTERRUPT(40)


.global tbl
tbl:
	.quad __entry0
	.quad __entry1
	.quad __entry2
	.quad __entry3
	.quad __entry4
	.quad __entry5
	.quad __entry6
	.quad __entry7
	.quad __entry8
	.quad __entry9
	.quad __entry10
	.quad __entry11
	.quad __entry12
	.quad __entry13
	.quad __entry14
	.quad __entry15
	.quad __entry16
	.quad __entry17
	.quad __entry18
	.quad __entry19
	.quad __entry20
	.quad __entry21
	.quad __entry22
	.quad __entry23
	.quad __entry24
	.quad __entry25
	.quad __entry26
	.quad __entry27
	.quad __entry28
	.quad __entry29
	.quad __entry30
	.quad __entry31
	.quad __entry32
	.quad __entry33
	.quad __entry34
	.quad __entry35
	.quad __entry36
	.quad __entry37
	.quad __entry38
	.quad __entry39
	.quad __entry40






	

	


