#ifndef __SYNC_H__
#define __SYNC_H__

#include <assert.h>
#include <lib.h>
#include <mmgr.h>
#include <naiveConsole.h>
#include <process.h>
#include <queue.h>
#include <scheduler.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_SEMS 767
#define MAX_USER_SEMS 256

int sem_open(uint16_t id, uint64_t value);

int sem_wait(uint16_t semID);

int sem_post(uint16_t semID);

int sem_close(uint16_t semID);

void printBlockedProcesses(uint16_t semId);

void sem_dump(void);

#endif
