[org 0x7E00]

stage2:
    mov si, stage2_msg
    call real_mode_print
    jmp $ ; Loop forever

%include "real_mode_print.inc"

stage2_msg: db "Loaded stage 2 bootloader", 13, 10, 0