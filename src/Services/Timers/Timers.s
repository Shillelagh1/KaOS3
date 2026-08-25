section .text
global _isr_heartbeat
_isr_heartbeat:
    cli
    pushad
    extern c_heartbeat
    call c_heartbeat
    popad
    sti
    iret