PRESENT  equ 1 << 7
NOT_SYS  equ 1 << 4
EXEC     equ 1 << 3
DC       equ 1 << 2
RW       equ 1 << 1
ACCESSED equ 1 << 0

GRAN_4K equ 1 << 7
SZ_32   equ 1 << 6
LM      equ 1 << 5

gdt64:
    .null: equ $ - gdt64
        dq 0
    
    .code: equ $ - gdt64
        dd 0xFFFF
        db 0
        db PRESENT | NOT_SYS | EXEC | RW
        db GRAN_4K | LM | 0xF
        db 0
    
    .data: equ $ - gdt64
        dd 0xFFFF
        db 0
        db PRESENT | NOT_SYS | RW
        db GRAN_4K | SZ_32 | 0xF
        db 0
    
    .tss: equ $ - gdt64
        dd 0x00000068
        dd 0x00CF8900
    
    .ptr:
        dw $ - gdt64 - 1
        dq gdt64