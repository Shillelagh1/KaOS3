global _pg_enable_paging
_pg_enable_paging:
    ; Program page directory address
    mov eax, [esp + 4]
    mov cr3, eax

    ; Enable PSE
    mov eax, cr4
    or eax, 0x10
    mov cr4, eax

    ; Enable paging
    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax

    ret