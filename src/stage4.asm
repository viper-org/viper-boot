[org 0x8200]
[bits 64]

stage4:
    cli
    call parse_phdr
    jmp $ ; In case kernel returns unexpectedly

%include "elf.inc"

times 512-($-$$) db 0