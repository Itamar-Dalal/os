BITS 16
ORG 0x7c00

start:
    jmp $

; Padding to make the total size 512 bytes
TIMES 510 - ($ - $$) db 0

; Boot sector signature (magic number)
dw 0xaa55
