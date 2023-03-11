gdt:
    dd 0
    dd 0

.code:
    dw 0xFFFF
    dw 0
    db 0
    db 0b10011010
    db 0b11001111
    db 0
.data:
    dw 0xFFFF
    dw 0
    db 0
    db 0b10010010
    db 0b11001111
    db 0

gdtr:
    dw $ - gdt - 1
    dd gdt