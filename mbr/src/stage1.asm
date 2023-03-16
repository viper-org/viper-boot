[org 0x7C00]

boot:
    xor ax, ax
    mov ds, ax ; Set DS register to 0
    cld

    mov si, startup_msg
    call real_mode_print ; Print startup message

    call read_disk ; Read stage 2 loader from disk
    jmp 0x7E00
    jmp $ ; If failed

%include "real_mode_disk.inc"
%include "real_mode_print.inc"

startup_msg: db "ViperBoot started. Reading stage 2 loader from disk", 13, 10, 0

times 510-($-$$) db 0
dw 0xAA55