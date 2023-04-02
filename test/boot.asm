;[section .bss]
;stack_bottom:
;    resb 0x1000
;stack_top:

[section .text]
[extern kmain]
[global _start]
_start:
    mov rax, 0x6969
    call kmain
    jmp $