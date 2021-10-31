#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MAX_PROCESS_COUNT 256
#define PROCESS_SIZE 8 * 1024

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define READ 0
#define WRITE 1

#define MASK_BACKGROUND (0x1)

typedef int16_t pid_t;

typedef enum Status { READY = 0, TERMINATED, BLOCKED } Status;

// ASM
uint64_t init_process(uint64_t stack_base, uint64_t rip, uint64_t argc,
                      uint64_t argv);
void _int20(void);

pid_t createProcess(uint64_t rip, uint8_t priority, char *name, uint64_t argc,
                    char *argv[], uint8_t mode);
int exec(pid_t pid);
int kill(pid_t pid);
int block(pid_t pid);
int unblock(pid_t pid);
void remove(pid_t pid);
void setRsp(pid_t pid, uint64_t rsp);
int setStatusToBlocked(pid_t pid);
int setPriority(pid_t pid, uint8_t priority);
uint64_t getRsp(pid_t pid);
int getPriority(pid_t pid);
uint8_t isReady(pid_t pid);
uint8_t isBlocked(pid_t pid);
uint8_t isTerminated(pid_t pid);
int dup(pid_t pid, int old, int new);
void showAllPs(void);

#endif