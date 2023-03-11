[bits 32]
stage3:
    mov ax, 0x10 ; Set segment registers
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ebp, 0x90000 ; Set up a basic stack
    mov esp, ebp

    mov BYTE [0xB8000], 'H' ; Do something 32 bit to make sure we are in PM
    jmp $ ; Loop forever