bits 32

; Multiboot header definitions
MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
MBFLAGS  equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)

; Other magic
VGA_BUF equ 0xB8000

;   DWORD page directory entry for identity mapping the first 4MiB of
; memory. Equates to an address of 0x0, is marked present, R/W(!), supervisor only,
; write-through, global, PS=1 (4MiB)
KERN_PDE equ 0x18B

; Multiboot header declaration
section .multiboot.data
	align 4
		dd MAGIC
		dd MBFLAGS
		dd CHECKSUM
    _launcher_msg:
        db 0x10, "KaOS Panic", 0x11, 0x00

section .bss
    align 4096
    _boot_pagedir:
        resb 4096
    _kernel_stack.bottom:
        resb 8192
    _kernel_stack.top:

; Lower half only startup code
section .multiboot.text
    ; Entry point for the kernel
    global _start
    _start:
        cli
       
        ; Print a panic message in case we get stuck here
        ; I dont know what getting stuck here *implies* but
.print_:
        mov eax, _launcher_msg  ; Setup print variables
        mov edx, VGA_BUF
.print_loop:
        mov cl, [eax]           ; Load the character
        cmp cl, 0
        je .print_loop.break    ; Break from the loop if the characters is 0
        mov [edx], cl           ; Put the character in the buffer
        add eax, 1
        add edx, 2              ; Increment the pointers
        jmp .print_loop         ; Loop
.print_loop.break:

        ; Populate the page directory
        lea ecx, [_boot_pagedir - 0xFFC00000]   ; Calculate initial offset
        lea eax, [ecx + 0 * 4]
        mov dword [eax], KERN_PDE
        lea eax, [ecx + 1023 * 4]
        mov dword [eax], KERN_PDE

        ; Load the page directory
        lea eax, [_boot_pagedir - 0xFFC00000]
        mov cr3, eax

        ; Enable 4 MiB Pages
        mov eax, cr4
        mov eax, 0x00000010
        mov cr4, eax

        ; Enable paging
.enablepg_:
        mov eax, cr0
        or eax, 0x80000001 ; Enable paging and WE
        mov cr0, eax

        ; Jump to the higher half
        mov eax, .dest0
        jmp eax

section .text
.dest0:

        ; Clear the identity mapped page
        lea ecx, [_boot_pagedir - 0xFFC00000]   ; Calculate initial offset
        lea eax, [ecx + 0 * 4]
        mov dword [eax], 0x0

        ; Flush TLB
        mov eax, cr3
        mov cr3, eax

        ; Move to the higher half stack
        mov esp, _kernel_stack.top

        ; Enter higher level kernel
        extern boot_c_setup
        push ebx
        call boot_c_setup
.e: 
        hlt
        jmp .e
