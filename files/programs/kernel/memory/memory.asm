extern kernel_main_stack
extern KernelMain

global CallKernelMain
CallKernelMain:
    mov rsp, kernel_main_stack + 1024 * 1024
    call KernelMain
.fin:
    hlt
    jmp .fin