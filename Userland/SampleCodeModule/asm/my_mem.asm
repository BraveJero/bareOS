GLOBAL alloc, free

free: ; void free(void *ptr);
    mov rax, 19
    int 80h
    ret

alloc: ; void *alloc(size_t size);
    mov rax, 18
    int 80h
    ret