#ifndef __TESTS_H__
#define __TEST_H__

#include <exceptions.h>
#include <lib.h>
#include <my_mem.h>
#include <my_process.h>
#include <my_sync.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <utils.h>

#define MAX_BLOCKS 128
#define MAX_MEMORY 0xB000000 // Should be around 80% of memory managed by the MM
#define MAX_PROCESSES                                                          \
  10 // Should be around 80% of the the processes handled by the kernel
#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID 10
#define SEM_ID2 11
#define MINOR_WAIT 1000000            // To prevent a process from flooding the screen
#define WAIT      10000000            // Long enough to see theese processes beeing run at least twice
#define TOTAL_PROCESSES 3    // In testPrio



enum State { ERROR, RUNNING, BLOCKED, KILLED };

typedef struct P_rq {
  pid_t pid;
  enum State state;
} p_rq;

void testMM(void);
void testPrs(void);
void testSync(void);
void testPrio(void);

#endif