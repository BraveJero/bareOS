#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <naiveConsole.h>
#include <stddef.h>
#include <stdint.h>
#include <sync.h>
#include <userland.h>
#include <video.h>

#include <lib.h>
#include <naiveConsole.h>

#define LEFT_SHIFT 0x2A
#define LEFT_SHIFT_FLAG 0b00000001
#define RIGHT_SHIFT 0x36
#define RIGHT_SHIFT_FLAG 0b00000010
#define LEFT_ALT 0x38

#define BUFFER_SIZE 256

#define EXCEPTION_COUNT 32
#define ZERO_EXCEPTION_ID 0x00
#define INVALID_OPCODE_ID 0x06

#define KEYBOARD_WRITE_SEM MAX_SEMS - 1
#define KEYBOARD_BLOCK_SEM MAX_SEMS - 2

void keyboard_handler(void);
long stdRead(char *buf, size_t count);
int initKeyboard(void);

#endif
