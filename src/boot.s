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

    ; We will need to create a higher half stack as well hngnghngtnrhnrg

section .data
    align 16
    _launcher_gdt_segments:
    ; Null Segdesc
    dq 0x0000000000000000
    ; Code Segdesc
    dq 0x00cf9f000000ffff
    ; Data Segdesc
    dq 0x00cf93000000ffff

    _launcher_gdt:
    dd _launcher_gdt_segments
    dw 0x17 ; dec 23. Size of the GDT - 1 (3 * 8 - 1)
    

section .text
    ; Entry point for the kernel
    global _start
    _start:
        cli
        ; Setup the stack
        mov esp, _stack_top
        mov ebp, _stack_top
        push ebx

        ; Setup the Launcher GDT
        mov eax, _launcher_gdt
        lgdt [eax]
        hlt

        ; Load segment registers
        jmp 0x08:.CS
        .CS:
        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        ret

        ; Populate the page table 

        call boot_c_setup
    .e:
        hlt
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


    ; ENTER HIGHER HALF:
    ; Instead of jumping to a *new* higher half address (dest) instead modify the 
    ; return address for _enter_higher_half to incorporate the higher half offset

    global _enter_higher_half
        _enter_higher_half:
        mov eax, [esp + 4]  ; Offset to jump to
        mov edi, [esp]      ; Store the return address
        add edi, eax        ; Calculate the target address
        mov [esp], edi      ; Store the new (offset) return address in the stack
        ret                 ; Return to setup including the new offset