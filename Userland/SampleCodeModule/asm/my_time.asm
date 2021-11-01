GLOBAL getticks, getseconds

getticks: ; uint64_t getticks(void)
    mov rax, 29
    int 80h
    ret

getseconds: ; uint64_t getseconds(void)
    mov rax, 29
    int 80h
    ret

