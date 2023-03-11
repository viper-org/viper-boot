[org 0x7E00]

stage2:
    mov si, stage2_msg
    call real_mode_print

    call enable_a20 ; Enable A20 line

    jmp $ ; Loop forever

%include "real_mode_print.inc"
%include "a20.inc"

stage2_msg: db "Loaded stage 2 bootloader", 13, 10, 0