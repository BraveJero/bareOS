#ifndef __STRING_H__
#define __STRING_H__

#include <stdint.h>

int8_t strcmp(const char *p1, const char *p2);
uint8_t strlen(const char *str);
void strcpy(char *s, char *t);
int parser(char *input, char *argv[]);

#endif