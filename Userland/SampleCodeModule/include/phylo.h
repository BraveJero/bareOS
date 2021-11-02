#ifndef __PHYLO_H__
#define __PHYLO_H__

#include <stdint.h>
#include <stddef.h>
#include <utils.h>
#include <lib.h>
#include <my_process.h>
#include <my_sync.h>
#include <my_mem.h>

#define SEM_ID 25
#define MAX_FILOS 10 // HARDCODED

#define HUNGRY 0
#define THINKING 1
#define EATING 2

#define CLEAN_BUF while(get_char() != '\n');

static pid_t phylosophers[MAX_FILOS] = {0};
static uint8_t state[MAX_FILOS] = {THINKING};
static uint8_t sems[MAX_FILOS] = {0};
static int active = 0;

void phylo(int argc, char *argv[]);

#endif 