[section .text]
[global _start]
_start:
    [extern kmain]
    call kmain
    jmp $

[section .data]
test_req:
    dq 0x7638c0c9cf567885
    dq 0x80430eef20c20f7a