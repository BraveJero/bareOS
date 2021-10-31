#ifndef __MY_MEM_H__
#define __MY_MEM_H__

#include <stddef.h>

void *alloc(size_t size);

void free(void *ptr);

#endif