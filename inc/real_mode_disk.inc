dap:          db 16
              db 0
.num_blocks:  dw 1
.buffer_addr: dd 0x7E00
.start_lba:   dq 1

read_disk:
    mov si, dap
    mov ah, 0x42
    mov dl, 0x80
    int 0x13
    jc .fail
    ret
.fail:
    mov si, disk_fail_msg
    call real_mode_print
    jmp $

disk_fail_msg: db "Failed to read segments from disk. Halt", 13, 10, 0