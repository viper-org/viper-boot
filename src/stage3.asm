[org 0x8000]
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

page_table equ 0x03
mapping_2m equ 0x83

enter_lm:
    mov eax, 0x1000
    mov WORD [eax], page_table | 0x2000
    mov WORD [0x1FF8], page_table | 0x2000
    mov cr3, eax
    mov WORD [0x2000], page_table | 0x3000
    mov WORD [0x2008], page_table | 0x4000
    mov WORD [0x2FF0], page_table | 0x3000
    mov WORD [0x2FF8], page_table | 0x4000

    mov di, 0x3000
    xor ax, ax
    mov cx, 0x400
.loop:
    mov BYTE [di], mapping_2m
    mov WORD [di + 3], ax
    add di, 8
    add ax, 2
    loop .loop

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    lgdt [gdt64.ptr]
    jmp 0x8:0x8200

%include "gdt64.inc"

times 512-($-$$) db 0