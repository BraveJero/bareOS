#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>
#include <process.h>

int8_t initScheduler();
pid_t addToReady(pid_t pid);
uint64_t scheduler(uint64_t rsp);
void checkCurrent(pid_t pid);
pid_t getCurrentPid(void);

#endif