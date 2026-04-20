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

extern boot_c_setup

; Lower half only startup code
section .multiboot.text
    ; Entry point for the kernel
    global _start
    _start:
        cli

        call boot_c_setup
.e: 
        hlt
        jmp .e
