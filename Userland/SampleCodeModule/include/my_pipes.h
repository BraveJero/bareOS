#ifndef __MY_PIPES_H__
#define __MY_PIPES_H__

#include <stdint.h>

int pipe(int16_t pipeID, int fds[2]);

int closePipe(uint8_t pipeID);

void pipe_dump(void);

#endif
