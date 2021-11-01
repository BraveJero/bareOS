#ifndef __STRING_H__
#define __STRING_H__

#include <stdint.h>

#define MAX_COMMAND 256
#define MAX_ARGS 10

int8_t strcmp(const char *p1, const char *p2);
uint8_t strlen(const char *str);
char *strchr(const char *s, char c);
void strcpy(char *s, char *t);
int parser(char *input, char *argv[]);
int findCmd(char *in, char *cmds[], int dim);

#endif