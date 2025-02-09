#ifndef MY_PROCESS_H
#define MY_PROCESS_H

#include <lib.h>
#include <stdint.h>
#include <utils.h>

#define FOREGROUND 0
#define BACKGROUND 1

typedef int16_t pid_t;

// ASM
void ps(void);
pid_t createPs(uint64_t rip, int argc, char *argv[], uint8_t mode);
int block(pid_t pid);
int unblock(pid_t pid);
int kill(pid_t pid);
pid_t getpid(void);
int nice(pid_t pid, int adjustment);
void exit();
void yield(void);
int dup(pid_t pid, int old, int new);
int exec(pid_t pid);
int waitPid(pid_t pid);

// C
pid_t processManager(uint64_t rip, int argc, char *argv[], uint8_t mode,
                     int new_in, int new_out);

#endif
