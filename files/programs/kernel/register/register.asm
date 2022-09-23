bits 64
section .text

global SetCS
SetCS:
    push rbp
    mov rbp, rsp
    mov rax, .next
    push rdi
    push rax
    o64 retf
.next:
    mov rsp, rbp
    pop rbp
    ret