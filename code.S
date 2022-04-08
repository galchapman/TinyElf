.global _start
.org 0x0000000000400078

message: .ascii "Hello, World!\n"
.equ length, . - message

.equ SYSCALL_EXIT, 60

_start:
	// write($stdout, $message, $length)
	// xor %rdi, %rdi
	inc %di
	movq %rdi, %rax
	movq $message, %rsi
	mov $length, %dl
	syscall
	// exit
	mov $SYSCALL_EXIT, %al
	xor %di, %di
	syscall
