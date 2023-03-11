real_mode_print:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    mov bh, 0x0
    int 0x10
    jmp real_mode_print
.done:
    ret