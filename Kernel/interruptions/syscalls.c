#include <syscalls.h>

typedef uint64_t (*PSysCall)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

typedef struct dateType {
  uint8_t year, month, day;
  uint8_t hour, minute, second;
} dateType;

int64_t sys_read(unsigned int fd, char *buf, size_t count);
int64_t sys_write(uint8_t fd, char *buffer, uint64_t count);
int64_t sys_prt_in_screen(char *buffer, uint64_t count);
uint64_t sys_date(dateType *pDate);
uint64_t sys_mem(uint64_t rdi, uint64_t rsi, uint8_t rdx);
void sys_ps(void);
pid_t sys_createPs(uint64_t rip, int argc, char *argv[], uint8_t mode);
int sys_block(pid_t pid);
int sys_unblock(pid_t pid);
int sys_kill(pid_t pid);
pid_t sys_getpid(void);
int sys_nice(pid_t pid, int priority);
void sys_exit(void);
void sys_yield(void);
int sys_sem_open(uint8_t id, uint64_t value);
int sys_sem_wait(uint8_t semID);
int sys_sem_post(uint8_t semID);
int sys_sem_close(uint8_t semID);
int sys_exec(pid_t pid);
int sys_dup(pid_t pid, int old, int new);

static PSysCall sysCalls[255] = {
    (PSysCall)&sys_read,          // 0
    (PSysCall)&sys_write,         // 1
    (PSysCall)&sys_date,          // 2
    (PSysCall)&sys_mem,           // 3
    (PSysCall)&sys_ps,            // 4
    (PSysCall)&sys_createPs,      // 5
    (PSysCall)&sys_block,         // 6
    (PSysCall)&sys_unblock,       // 7
    (PSysCall)&sys_kill,          // 8
    (PSysCall)&sys_getpid,        // 9
    (PSysCall)&sys_nice,          // 10
    (PSysCall)&sys_exit,          // 11
    (PSysCall)&sys_yield,         // 12
    (PSysCall)&sys_sem_open,      // 13
    (PSysCall)&sys_sem_post,      // 14
    (PSysCall)&sys_sem_wait,      // 15
    (PSysCall)&sys_sem_close,     // 16
    (PSysCall)&sys_exec,          // 17
    (PSysCall)&alloc,             // 18
    (PSysCall)&free,              // 19
    (PSysCall)&sem_dump,          // 20
    (PSysCall)&pipe,              // 21
    (PSysCall)&closePipe,         // 22
    (PSysCall)&sys_dup,           // 23
    (PSysCall)&pipe_dump,         // 24
    (PSysCall)&mem_dump,          // 25
    (PSysCall)&sys_prt_in_screen, // 26
    (PSysCall)&plugPipe           // 27
};

uint64_t sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                           uint64_t rcx, uint64_t r8, uint64_t rax) {
  PSysCall sysCall = sysCalls[rax];
  if (sysCall != 0)
    return sysCall(rdi, rsi, rdx, rcx, r8);
  return 0;
}

int64_t sys_write(uint8_t fd, char *buffer, uint64_t count) {
  if (buffer == NULL || count == 0)
    return -1;

  if (fd == STDIN_FILENO)
    return -1;

  if (fd == STDOUT_FILENO) {
    fd = getFd(getCurrentPid(), STDOUT_FILENO);
  }

  if (fd > 3)
    return pipeWrite(fd, buffer, count);

  Color *fontColor = (fd == STDERR_FILENO) ? &RED : &WHITE;

  int i;
  for (i = 0; i < count && buffer[i]; i++)
    ncPrintCharAtt(buffer[i], fontColor, &BLACK);

  return i;
}

int64_t sys_prt_in_screen(char *buffer, uint64_t count) {
  if (buffer == NULL || count == 0)
    return -1;

  if (getFd(getCurrentPid(), STDIN_FILENO) != STDIN_FILENO) {
    return 0;
  }

  int i;
  for (i = 0; i < count && buffer[i]; i++)
    ncPrintCharAtt(buffer[i], &WHITE, &BLACK);

  return i;
}

int64_t sys_read(unsigned int fd, char *buf, size_t count) {
  if (fd == STDERR_FILENO || fd == STDOUT_FILENO)
    return -1;

  if (buf == NULL || count == 0)
    return -1;

  if (fd == STDIN_FILENO) {
    fd = getFd(getCurrentPid(), STDIN_FILENO);
  }

  if (fd == STDIN_FILENO)
    return stdRead(buf, count);

  return pipeRead(fd, buf, count);
}

uint8_t BCDToDec(uint8_t bcd) {
  return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

uint64_t sys_date(dateType *pDate) {
  if (pDate == 0)
    return 0;
  pDate->year = BCDToDec(getYear());
  pDate->month = BCDToDec(getMonth());
  pDate->day = BCDToDec(getDay());
  pDate->hour = BCDToDec(getHour());
  pDate->minute = BCDToDec(getMinute());
  pDate->second = BCDToDec(getSecond());
  return 1;
}

uint64_t sys_mem(uint64_t rdi, uint64_t rsi, uint8_t rdx) {
  // qemu tiene 64GB mapeados en memoria, asi que en el emulador
  // incluso con sólo 512MB de memoria
  // Podés acceder a todas las direcciones hasta 0x1000000000 - 1
  if (rdi >= 0x1000000000 || rdi - 1 + rdx >= 0x1000000000)
    return 1;

  uint8_t *src = (uint8_t *)rdi;
  uint8_t *dst = (uint8_t *)rsi;

  uint8_t i;
  for (i = 0; i < rdx; i++)
    dst[i] = src[i];

  return i;
}

void sys_ps(void) { showAllPs(); }

pid_t sys_createPs(uint64_t rip, int argc, char *argv[], uint8_t mode) {
  return createProcess(rip, 0, argc, argv, mode);
}

int sys_block(pid_t pid) { return block(pid); }

int sys_unblock(pid_t pid) { return unblock(pid); }

int sys_kill(pid_t pid) { return kill(pid); }

pid_t sys_getpid(void) { return getCurrentPid(); }

int sys_nice(pid_t pid, int adjustment) {
  int8_t old = getPriority(pid);
  int8_t new = old + adjustment;
  if (new < 0)
    new = 0;
  return setPriority(pid, new);
}

void sys_exit(void) { kill(getCurrentPid()); }

void sys_yield(void) { yield_cpu(); }

int sys_sem_open(uint8_t id, uint64_t value) { return sem_open(id, value); }

int sys_sem_wait(uint8_t semID) { return sem_wait(semID); }

int sys_sem_post(uint8_t semID) { return sem_post(semID); }

int sys_sem_close(uint8_t semID) { return sem_close(semID); }

int sys_exec(pid_t pid) { return exec(pid); }

int sys_dup(pid_t pid, int old, int new) { return dup(pid, old, new); }
