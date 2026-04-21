bits 32

; Multiboot header definitions
MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
MBFLAGS  equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)

VGA_BUF equ 0xB8000

; Multiboot header declaration
section .multiboot.data
	align 4
		dd MAGIC
		dd MBFLAGS
		dd CHECKSUM
    _launcher_msg:
        dd 0x10, "KaOS Panic", 0x11, 0x00

section .launcher.stack
_launcher_stack_bottom:
resb 8192

extern boot_c_setup

; Lower half only startup code
section .multiboot.text
    ; Entry point for the kernel
    global _start
    _start:
        cli

        mov ebp, _launcher_stack_bottom
        mov esp, _launcher_stack_bottom
        push ebp
       
        ; Print
        mov eax, _launcher_msg  ; Setup print variables
        mov ebx, VGA_BUF
.print_loop:
        mov ecx, [eax]          ; Load the character
        cmp ecx, 0
        je .print_loop.break    ; Break from the loop if the characters is 0
        mov [ebx] ecx           ; Put the character in the buffer
        add eax, 1
        add ebx, 2              ; Increment the pointers
        jmp .print_loop         ; Loop
.print_loop.break:

.e: 
        hlt
        jmp .e
