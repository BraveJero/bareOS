#ifndef __UTILS_H__
#define __UTILS_H__

#include <ctype.h>
#include <stddef.h>

#define BYTES_TO_READ 32

void printDate(void);
void help(void);
void printmem(void);
void printFeatures(void);
void printQuadraticRoots(void);
uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
void *memset(void *s, int c, size_t n);
unsigned long _Stoul(const char *s, char **endptr, int base);

#endif