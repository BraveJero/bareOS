#ifndef MY_SYNC_H
#define MY_SYNC_H

#include <stdint.h>

void sem_dump(void);

int sem_open(uint8_t semId, uint64_t value);

int sem_wait(uint8_t semId);

int sem_post(uint8_t semId);

int sem_close(uint8_t semId);

#endif
