#include <pipe.h>

#define BUFFER_SIZE 256 // Defined locally to use it as a cyclic buffer.

typedef struct pipe {
  char buf[BUFFER_SIZE];
  uint8_t r_pointer, w_pointer;
  uint16_t write, lock;
  pid_t users[MAX_USERS];
  uint8_t userCount;
} Pipe;

static Pipe *pipes[MAX_PIPES] = {0};
static uint8_t waiting = 0;

static nextFreeIdx(void) {
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
    pipes[pipeID]->users[pipes[pipeID]->userCount++] = getCurrentPid();
    fds[0] = 2 * pipeID + 3;
    fds[1] = 2 * pipeID + 4;
    pipes[pipeID]->write = sem_open(MAX_USER_SEMS + pipeID, 0);
    if (pipes[pipeID]->write == -1) {
      return -1;
    }

    pipes[pipeID]->lock = sem_open((MAX_USER_SEMS * 2) + pipeID, 1);
    if (pipes[pipeID]->lock == -1) {
      sem_close(pipes[pipeID]->write);
      return -1;
    }

    return pipes[pipeID]->userCount;
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

  pipes[pipeID]->users[pipes[pipeID]->userCount++] = getCurrentPid();
  fds[0] = 2 * pipeID + 3; // Avoid 0, 1 and 2 since they belong to 
  fds[1] = 2 * pipeID + 4; // STDIN, STDOUT and STDERR respectively.
  return pipes[pipeID]->userCount;
}

static int isInUsers(uint8_t pipeID, pid_t pid) {
  for (uint8_t i = 0; i < MAX_USERS; i++)
    if (pipes[pipeID]->users[i] == pid)
      return 1;

  return 0;
}

int pipeRead(int fd, char *buf, size_t count) {
  fd -= 3;
  if (buf == NULL || count == 0 || fd < 0)
    return -1;
  if (fd % 2 || pipes[fd / 2] == NULL ||
      !isInUsers(fd / 2, getCurrentPid())) { // TODO: Make a func isValid
    return -1;
  }
  fd = fd / 2;

  if (count > BUFFER_SIZE) {
    return 0;
  }

  long i = 0;

  if (sem_wait(pipes[fd]->lock) < 0)
    return -1;

  while (pipes[fd]->r_pointer == pipes[fd]->w_pointer) {
    waiting++;
    if (sem_post(pipes[fd]->lock) < 0)
      return -1;
    if (sem_wait(pipes[fd]->write) < 0)
      return -1;
    if (sem_wait(pipes[fd]->lock) < 0)
      return -1;
  }

  while (i < count && pipes[fd]->r_pointer != pipes[fd]->w_pointer)
    buf[i++] = pipes[fd]->buf[pipes[fd]->r_pointer++];



  if (sem_post(pipes[fd]->lock) < 0)
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
  if (fd % 2 == 0 || pipes[(fd - 1) / 2] == NULL ||
      !isInUsers((fd - 1) / 2, getCurrentPid())) {
    return -1;
  }
  fd = (fd - 1) / 2; // TODO: Check truncation.

  long i = 0;

  if (sem_wait(pipes[fd]->lock) < 0)
    return -1;
  
  while (i < count && buf[i])
    pipes[fd]->buf[pipes[fd]->w_pointer++] = buf[i++];

  while (waiting){
    waiting--;
    if (sem_post(pipes[fd]->write) < 0){
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
    pid_t currentPid = getCurrentPid();
    uint8_t i = 0;
    while (pipes[pipeID]->users[i] != currentPid &&
           i < pipes[pipeID]->userCount)
      i++;
    if (i == pipes[pipeID]->userCount)
      return -1;
    while (i < pipes[pipeID]->userCount - 1) {
      pipes[pipeID]->users[i] = pipes[pipeID]->users[i + 1];
      i++;
    }
    pipes[pipeID]->userCount--;
    sem_close(pipes[pipeID]->lock);
    sem_close(pipes[pipeID]->write);
    return 0;
  }
  sem_close(pipes[pipeID]->lock);
  sem_close(pipes[pipeID]->write);
  free(pipes[pipeID]);
  pipes[pipeID] = NULL;
}