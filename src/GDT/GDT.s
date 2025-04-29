section .text
_setGDTR:
    global _setGDTR

    ; Setup GDT
    mov eax, [esp + 4]
    lgdt [eax]

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