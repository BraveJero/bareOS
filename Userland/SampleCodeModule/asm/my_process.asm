GLOBAL ps, createPs, block, unblock, kill, getpid, nice, exit, yield, exec

ps: ; void ps void(void);
    mov rax, 4
    int 80h
    ret

createPs: ; pid_t createPs(uint64_t rip, char *name, int argc, char *argv[], uint8_t mode);
    mov rax, 5
    int 80h
    ret

block: ;int blockPs(pid_t pid);
    mov rax, 6
    int 80h
    ret

unblock: ; int unblockPs(pid_t pid);
    mov rax, 7
    int 80h
    ret

kill: ; int unblockPs(pid_t pid);
    mov rax, 8
    int 80h
    ret

getpid: ; pid_t getpid(void);
    mov rax, 9
    int 80h
    ret

nice: ; int nice(pid_t pid, int adjustment);
    mov rax, 10
    int 80h
    ret

exit: ; void exit(void);
    mov rax, 11
    int 80h
    ret

yield: ; void yield(void);
    mov rax, 12
    int 80h
    ret

exec: ; int exec(pid_t pid);
    mov rax, 17
    int 80h
    ret

dup: ; int dup(pid_t pid, int old, int new);
    mov rax, 23
    int 80h
    ret
