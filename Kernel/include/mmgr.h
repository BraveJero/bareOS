#ifndef MEMORY_ADT_H
#define MEMORY_ADT_H

#include <stddef.h>
#include <stdint.h>
#include <naiveConsole.h>

#define TOTAL_MEMORY 0x20000000
#define TOTAL_HEAP_SIZE 0x10000000

#define MINIMUM_ALLOCABLE_SIZE ((size_t)(blockLinkSize << 1))

#define BYTE_ALIGNMENT 8
#define BYTE_ALIGNMENT_MASK 0x0007

int initMgr(void);

void *alloc(size_t size);

void free(void *ptr);

void mem_dump(void);

#endif // MEMORY_ADT_H