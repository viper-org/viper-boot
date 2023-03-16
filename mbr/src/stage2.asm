[org 0x7E00]

stage2:
    mov si, stage2_msg
    call real_mode_print

    call enable_a20 ; Enable A20 line

    call enter_pm

    jmp $ ; Loop forever

enter_pm:
    cli
    lgdt [gdtr32]
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp 0x8:0x8000

%include "real_mode_print.inc"
%include "a20.inc"
%include "gdt32.inc"

stage2_msg: db "Loaded stage 2 bootloader", 13, 10, 0

times 512-($-$$) db 0