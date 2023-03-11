enable_a20:
    mov ax, 0x2402
    int 0x15
    jb .fail
    cmp ah, 0
    jnz .fail

    cmp al, 1
    jz .done

    mov ax, 0x2401
    int 0x15
    jb .fail
    cmp ah, 0
    jnz .fail

.done:
    ret

.fail:
    mov si, a20_fail_msg
    call real_mode_print

a20_fail_msg: db "Failed to activate A20 line. Halt", 13, 10, 0