#ifndef __TESTS_H__
#define __TEST_H__

#include <exceptions.h>
#include <lib.h>
#include <my_process.h>
#include <my_mem.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <utils.h>

#define MAX_BLOCKS 128
#define MAX_MEMORY 0xB000000 // Should be around 80% of memory managed by the MM
#define MAX_PROCESSES                                                          \
  200 // Should be around 80% of the the processes handled by the kernel

enum State { ERROR, RUNNING, BLOCKED, KILLED };

typedef struct P_rq {
  pid_t pid;
  enum State state;
} p_rq;

void testMM(void);
void testPrs(void);

#endif