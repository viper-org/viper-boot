    e_entry equ 0x18
    e_phoff equ 0x20
    e_phentsize equ 0x36
    e_phnum equ 0x38

    p_type equ 0x0
    p_offset equ 0x8
    p_vaddr equ 0x10
    p_filesz equ 0x20
    p_memsz equ 0x28

    elf_header equ 0x8400

parse_phdr:
    xor al, al

    mov r8, [elf_header + e_phoff]
    add r8, elf_header
    movzx r11, WORD [elf_header + e_phnum]
    movzx r9, WORD [elf_header + e_phentsize]

.loop:
    cmp BYTE [r8 + p_type], 1
    jne .loop2
    
    mov rdi, [r8 + p_vaddr]
    mov rsi, [r8 + p_offset]
    add rsi, elf_header

    mov rcx, [r8 + p_filesz]
    mov r12, rcx
    rep movsb

    mov rcx, [r8 + p_memsz]
    sub rcx, r12
    rep stosb

.loop2:
    add r8, r9
    dec r11
    jnz .loop

    jmp [elf_header + e_entry]