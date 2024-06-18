BITS 16
ORG 0x7c00

mov si, msg
call prints
jmp $

prints:
	mov ah, 0x0e
	printc:
		mov al, [si]
		inc si
		cmp al, 0
		jz return
		int 0x10
		jmp printc
	return:
		ret

msg db "Hello World!", 0

TIMES 510 - ($ - $$) db 0 ; Padding to make the total size 512 bytes
dw 0xaa55 ; Boot sector signature (magic number)
