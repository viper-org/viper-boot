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

    mov BYTE [0xB8000], 'H' ; Do something 32 bit to make sure we are in PM

enter_lm:
    mov edi, 0x1000
    mov cr3, edi
    xor eax, eax
    mov ecx, 0x1000
    rep stosd
    mov edi, cr3

    mov DWORD [edi], 0x2003
    add edi, 0x1000
    
    mov DWORD [edi], 0x3003
    add edi, 0x1000

    mov DWORD [edi], 0x4003
    add edi, 0x1000

    mov ebx, 0x3
    mov ecx, 0x200

.set_entry:
    mov DWORD [edi], ebx
    add ebx, 0x1000
    add edi, 8
    loop .set_entry

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