section .text
_setIDTR:
    global _setIDTR

    ; Setup IDT
    mov eax, [esp + 4]
    lidt [eax]

    mov al, 0x11
    out 0x20, al
    out 0xA0, al	; Make PICs listen.
    
    mov al, 0x20
    out 0x21, al
    mov al, 0x28
    out 0xA1, al	; Set offset.
    
    mov al, 4
    out 0x21, al
    mov al, 2
    out 0xA1, al	; Configure for slavery.
    
    mov al, 1
    out 0xA1, al
    out 0x21, al	; Configure to 8086.
    
    mov al, 0xFF
    out 0x21, al
    out 0xA1, al
    ret