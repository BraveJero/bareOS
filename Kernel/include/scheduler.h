#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <interrupts.h>
#include <naiveConsole.h>
#include <process.h>
#include <queue.h>
#include <stddef.h>
#include <stdint.h>

int8_t initScheduler();
pid_t addToReady(pid_t pid);
uint64_t scheduler(uint64_t rsp);
void yield_cpu(void);
pid_t getCurrentPid(void);

#endif