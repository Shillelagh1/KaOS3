section .text
global _isr_ps2kbd
_isr_ps2kbd:
    cli
    pushad
    extern c_isr_ps2kbd
    call c_isr_ps2kbd
    popad
    sti
    iret