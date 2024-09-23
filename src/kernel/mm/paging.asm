section .text
    global loadPageDirectory
    global enablePaging

; Function to load the page directory into CR3
loadPageDirectory:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]  ; Get the address of the page directory from the stack
    mov cr3, eax        ; Load the address into CR3
    pop ebp
    ret

; Function to enable paging by setting the paging bit in CR0
enablePaging:
    push ebp
    mov ebp, esp
    mov eax, cr0        ; Read the current value of CR0
    or eax, 0x80000000  ; Set the paging bit (bit 31)
    mov cr0, eax        ; Write the new value back to CR0
    pop ebp
    ret
