#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stddef.h>
#include <stdint.h>
#include <sync.h>

#define KEYBOARD_WRITE_SEM MAX_SEMS - 1
#define KEYBOARD_BLOCK_SEM MAX_SEMS - 2

void keyboard_handler(void);
long stdRead(char *buf, size_t count);
int initKeyboard(void);

#endif
