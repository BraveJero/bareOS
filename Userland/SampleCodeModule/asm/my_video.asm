GLOBAL clear

clear: ; void clear(void)
    mov rax, 28
    int 80h
    ret
