GLOBAL alloc, free, mem_dump

free: ; void free(void *ptr);
    mov rax, 19
    int 80h
    ret

alloc: ; void *alloc(size_t size);
    mov rax, 18
    int 80h
    ret

mem_dump: ; void mem_dump(void);
    mov rax, 25
    int 80h
    ret