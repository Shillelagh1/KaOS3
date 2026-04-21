bits 32

; Multiboot header definitions
MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
MBFLAGS  equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)

; Multiboot header declaration
section .multiboot.data
	align 4
		dd MAGIC
		dd MBFLAGS
		dd CHECKSUM

section .launcher.stack
_launcher_stack_bottom:
resb 8192

extern boot_c_setup

; Lower half only startup code
section .multiboot.text
    ; Entry point for the kernel
    global _start
    _start:
        hlt
        cli

        mov ebp, _launcher_stack_bottom
        mov esp, _launcher_stack_bottom
        push ebp

        mov [0xB8000], 0x10
        mov [0xB8002], 0x11
        mov [0xB8004], 0x4B
        mov [0xB8006], 0x61
        mov [0xB8008], 0x4F
        mov [0xB800A], 0x53

        call boot_c_setup
.e: 
        hlt
        jmp .e
