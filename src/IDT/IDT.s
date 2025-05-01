section .text
_setIDTR:
    global _setIDTR

    ; Setup IDT
    mov eax, [esp + 4]
    lidt [eax]
    
    ret