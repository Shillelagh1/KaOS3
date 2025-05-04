bits 32

extern boot_c_setup

; Multiboot header
MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
MBFLAGS  equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)

section .multiboot
	align 4
		dd MAGIC
		dd MBFLAGS
		dd CHECKSUM

section .bss
    ; Create a temporary stack
	align 16
    global _stack_top
	_stack_bottom:
	    resb 16384 ; 16 KiB
	    _stack_top:

section .text
    ; Entry point for the kernel
    global _start
    _start:
        cli
        mov esp, _stack_top
        mov ebp, _stack_top
        push ebx
        call boot_c_setup
    .e:
        jmp .e
    global _isr_heartbeat
        _isr_heartbeat:
        cli
        pushad
        extern heartbeat
        call heartbeat
        popad
        sti
        iret