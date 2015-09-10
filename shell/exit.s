.section .text

_start:
	mov $0x0, %rax
	mov $0x0, %rdi
	mov %rsp, %rsi
	mov $0x0, %rdx
	syscall
	.global _start
.end
