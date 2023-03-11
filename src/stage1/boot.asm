[org 0x7C00]

boot:
    xor ax, ax
    mov ds, ax
    cld

    mov si, startup_msg
    call real_mode_print
    jmp $

startup_msg: db "ViperBoot started successfully.", 13, 10, 0

%include "real_mode_print.inc"

times 510-($-$$) db 0
dw 0xAA55