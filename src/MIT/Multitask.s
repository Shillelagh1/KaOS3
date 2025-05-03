extern MIT_heartbeat

global _isr_MIT_heartbeat
    _isr_MIT_heartbeat:
    cli
    pushad
    call MIT_heartbeat
    popad
    sti
    iret