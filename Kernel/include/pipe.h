#ifndef __PIPE_H__
#define __PIPE_H__

#include <stdint.h>
#include <stddef.h>
#include <process.h>
#include <scheduler.h>
#include <mmgr.h>
#include <sync.h>
#include <naiveConsole.h>

#define MAX_USERS 16  // Users per pipe
#define MAX_PIPES 256

int pipe(int16_t pipeID, int fds[2]);

int pipeRead(int fd, char *buf, size_t count);

int pipeWrite(int fd, const char *buf, size_t count);

int closePipe(uint8_t pipeID);

void pipe_dump(void);

int plugPipe(uint8_t pipeID);

#endif