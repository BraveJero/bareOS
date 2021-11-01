GLOBAL pipe, closePipe, pipe_dump

pipe: ;; int pipe(int16_t pipeID, int fds[2]);
  mov rax, 21
  int 80h
  ret

closePipe: ;; int closePipe(uint8_t pipeID);
  mov rax, 22
  int 80h
  ret


pipe_dump: ;; void pipe_dump(void);
  mov rax, 24
  int 80h
  ret