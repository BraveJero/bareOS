// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pipe.h>

#define PIPE_BUFFER_SIZE 65536 // Defined locally to use it as a cyclic buffer.

typedef struct pipe {
  char buf[PIPE_BUFFER_SIZE];
  uint16_t r_pointer, w_pointer;
  uint16_t write, lock, reading;
  uint8_t userCount;
} Pipe;

static Pipe *pipes[MAX_PIPES] = {0};

static int nextFreeIdx(void) {
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i] == NULL) {
      return i;
    }
  }
  return -1;
}

int pipe(int16_t pipeID, int fds[2]) {
  if (pipeID > MAX_PIPES)
    return -1;
  if (pipeID == -1) {
    pipeID = nextFreeIdx();
    if (pipeID < 0)
      return pipeID;
  } else if (pipes[pipeID]) {
    pipes[pipeID]->userCount++;
    fds[0] = 2 * pipeID + 3;
    fds[1] = 2 * pipeID + 4;
    pipes[pipeID]->write = sem_open(MAX_USER_SEMS + pipeID, 0);
    if (pipes[pipeID]->write == -1) {
      return -1;
    }

    pipes[pipeID]->lock = sem_open((MAX_USER_SEMS + MAX_PIPES) + pipeID, 1);
    if (pipes[pipeID]->lock == -1) {
      sem_close(pipes[pipeID]->write);
      return -1;
    }

    pipes[pipeID]->reading =
        sem_open((MAX_USER_SEMS + (MAX_PIPES * 2)) + pipeID, 1);
    if (pipes[pipeID]->reading == -1) {
      sem_close(pipes[pipeID]->write);
      sem_close(pipes[pipeID]->lock);
      return -1;
    }

    return pipeID;
  }
  if ((pipes[pipeID] = alloc(sizeof(Pipe))) == NULL) {
    return -1;
  }

  pipes[pipeID]->userCount = pipes[pipeID]->r_pointer =
      pipes[pipeID]->w_pointer = 0;
  pipes[pipeID]->write = sem_open(MAX_USER_SEMS + pipeID, 0);
  if (pipes[pipeID]->write == -1) {
    return -1;
  }

  pipes[pipeID]->lock = sem_open((MAX_USER_SEMS + MAX_PIPES) + pipeID, 1);
  if (pipes[pipeID]->lock == -1) {
    sem_close(pipes[pipeID]->write);
    return -1;
  }

  pipes[pipeID]->reading =
      sem_open((MAX_USER_SEMS + (MAX_PIPES * 2)) + pipeID, 1);
  if (pipes[pipeID]->reading == -1) {
    sem_close(pipes[pipeID]->write);
    sem_close(pipes[pipeID]->lock);
    return -1;
  }

  pipes[pipeID]->userCount++;
  fds[0] = 2 * pipeID + 3; // Avoid 0, 1 and 2 since they belong to
  fds[1] = 2 * pipeID + 4; // STDIN, STDOUT and STDERR respectively.
  return pipeID;
}

int pipeRead(int fd, char *buf, size_t count) {
  fd -= 3;
  if (buf == NULL || count == 0 || fd < 0)
    return -1;
  if (fd % 2 || pipes[fd / 2] == NULL) {
    return -1;
  }
  fd = fd / 2;

  if (count > PIPE_BUFFER_SIZE) {
    return 0;
  }

  long i = 0;

  if (sem_wait(pipes[fd]->reading) < 0)
    return -1;

  while (i < count) {
    if (sem_wait(pipes[fd]->write) < 0)
      return -1;

    if (pipes[fd]->buf[pipes[fd]->r_pointer] == EOF) {
      pipes[fd]->r_pointer++;
      break;
    }

    if (pipes[fd]->buf[pipes[fd]->r_pointer] == '\b') {
      if (i > 0) {
        i--;
      }
    } else {
      buf[i++] = pipes[fd]->buf[pipes[fd]->r_pointer];
      if (buf[i - 1] == '\n') {
        pipes[fd]->r_pointer++;
        break;
      }
    }
    pipes[fd]->r_pointer++;
  }

  if (sem_post(pipes[fd]->reading) < 0)
    return -1;

  if (i < count) {
    buf[i] = '\0';
  }

  return i;
}

int pipeWrite(int fd, const char *buf, size_t count) {
  fd -= 3;
  if (buf == NULL || count == 0 || fd < 0)
    return -1;
  if (fd % 2 == 0 || pipes[(fd - 1) / 2] == NULL) {
    return -1;
  }
  fd = fd / 2;

  long i = 0;

  if (sem_wait(pipes[fd]->lock) < 0)
    return -1;

  while (i < count && buf[i]) {
    pipes[fd]->buf[pipes[fd]->w_pointer++] = buf[i++];
    if (sem_post(pipes[fd]->write) < 0) {
      return -1;
    }
  }

  if (sem_post(pipes[fd]->lock) < 0)
    return -1;

  return i;
}

int closePipe(uint8_t pipeID) {
  if (pipes[pipeID] == NULL) {
    return -1;
  }
  if (pipes[pipeID]->userCount > 1) {
    pipes[pipeID]->userCount--;
    sem_close(pipes[pipeID]->lock);
    sem_close(pipes[pipeID]->write);
    return 0;
  }
  sem_close(pipes[pipeID]->lock);
  sem_close(pipes[pipeID]->write);
  sem_close(pipes[pipeID]->reading);
  free(pipes[pipeID]);
  pipes[pipeID] = NULL;
  return 0;
}

int plugPipe(uint8_t pipeID) {
  if (pipes[pipeID] == NULL) {
    return -1;
  }

  if (sem_wait(pipes[pipeID]->lock) < 0)
    return -1;

  pipes[pipeID]->buf[pipes[pipeID]->w_pointer++] = EOF;
  if (sem_post(pipes[pipeID]->write) < 0) {
    return -1;
  }

  if (sem_post(pipes[pipeID]->lock) < 0)
    return -1;

  return 0;
}

void pipe_dump(void) {
  ncNewline();
  ncPrint("---------------------------------------");
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i] != NULL) {
      ncNewline();
      ncPrint("Pipe ID: ");
      ncPrintDec(i);
      ncNewline();
      ncPrint("Processes using this pipe: ");
      ncPrintDec(pipes[i]->userCount);
      ncNewline();
      printBlockedProcesses(pipes[i]->write);
      ncNewline();
      ncPrint("Pipe content in bytes: ");
      for (int j = pipes[i]->r_pointer; j < pipes[i]->w_pointer;
           j = (j + 1) % PIPE_BUFFER_SIZE) {
        ncPrintChar(pipes[i]->buf[j]);
      }
      ncNewline();
      ncPrint("---------------------------------------");
    }
  }
  ncNewline();
}