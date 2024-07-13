%macro ISR_NOERRCODE 1
	global isr%1
	isr%1:
		cli ; Blocks sending further interrupts
		push dword 0 ; dword - int (32 bits)
		push dword %1
		jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
	global isr%1
	isr%1:
		cli
		push dword %1
		jmp isr_common_stub
%endmacro

%macro IRQ 2
	global irq%1
	irq%1:
		cli
		push dword 0
		push dword %2
		jmp irq_common_stub
%endmacro

%macro ISR_LOOP_NOERRCODE 2
%assign i %1
%rep %2
    ISR_NOERRCODE i
    %assign i i+1
%endrep
%endmacro

%macro ISR_LOOP_ERRCODE 2
%assign i %1
%rep %2
    ISR_ERRCODE i
    %assign i i+1
%endrep
%endmacro

ISR_LOOP_NOERRCODE 0, 8    ; ISRs 0-7
ISR_ERRCODE 8              ; ISR 8
ISR_NOERRCODE 9            ; ISR 9
ISR_LOOP_ERRCODE 10, 5     ; ISRs 10-14
ISR_LOOP_NOERRCODE 15, 17 ; ISRs 15-31
IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47
ISR_LOOP_NOERRCODE 48, 208 ; ISRs 48-255

extern isr_handler

isr_common_stub:
	pusha

	mov ax, ds
	push eax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call isr_handler

	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx

	popa

	add esp, 8
	sti
	iret

extern irq_handler

irq_common_stub:
	pusha

	mov ax, ds
	push eax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call irq_handler

	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx

	popa

	add esp, 8
	sti
	iret