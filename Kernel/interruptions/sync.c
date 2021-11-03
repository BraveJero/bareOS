// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <sync.h>

typedef struct semaphore *sem_t;

typedef struct semaphore {
  uint64_t value, activeCount;
  int64_t mutex;
  Queue blockedQueue;
} Semaphore;

static sem_t semaphores[MAX_SEMS] = {NULL};

int sem_open(uint16_t id, uint64_t value) {
  if (semaphores[id]) {
    acquire(&(semaphores[id]->mutex));
    semaphores[id]->activeCount++;
    release(&(semaphores[id]->mutex));
    return id;
  }
  semaphores[id] = alloc(sizeof(Semaphore));
  if (semaphores[id] == NULL) {
    return -1;
  }
  semaphores[id]->value = value;
  semaphores[id]->activeCount = 1;
  semaphores[id]->mutex = 0;
  semaphores[id]->blockedQueue = newQueue();

  return id;
}

int sem_wait(uint16_t semID) {
  if (semaphores[semID] == NULL) {
    return -1;
  }
  acquire(&(semaphores[semID]->mutex));
  if (semaphores[semID]->value > 0) {
    semaphores[semID]->value--;
    release(&(semaphores[semID]->mutex));
    return 0;
  }
  pid_t currentPid = getCurrentPid();
  int ans = push(semaphores[semID]->blockedQueue, currentPid);
  setStatusToBlocked(currentPid);
  release(&(semaphores[semID]->mutex));
  if (ans < 0) {
    setStatusToReady(currentPid);
    return -1;
  }
  yield_cpu();
  return 0;
}

int sem_post(uint16_t semID) {
  if (semaphores[semID] == NULL) {
    return -1;
  }
  acquire(&(semaphores[semID]->mutex));
  if (queueSize(semaphores[semID]->blockedQueue) == 0) {
    semaphores[semID]->value++;
    release(&(semaphores[semID]->mutex));
    return 0;
  }
  pid_t toUnblockPid = pop(semaphores[semID]->blockedQueue);
  release(&(semaphores[semID]->mutex));
  if (toUnblockPid < 0) {
    return -1;
  }
  unblock(toUnblockPid);
  return 0;
}

int sem_close(uint16_t semID) {
  if (semaphores[semID] == NULL) {
    return -1;
  }
  acquire(&(semaphores[semID]->mutex));
  semaphores[semID]->activeCount--;
  if (semaphores[semID]->activeCount > 0) {
    release(&(semaphores[semID]->mutex));
    return semaphores[semID]->activeCount;
  }
  free(semaphores[semID]);
  semaphores[semID] = NULL;
  release(&(semaphores[semID]->mutex));
  return 0;
}

void printBlockedProcesses(uint16_t semId) {
  if(semId >= MAX_SEMS || semaphores[semId] == NULL)
    return;
    
  ncPrint("Blocked processes: ");
  printQueue(semaphores[semId]->blockedQueue);
}

void sem_dump(void) {
  ncNewline();
  ncPrint("---------------------------------------");
  for(uint16_t i = 0; i < MAX_SEMS; i++) {
    if(semaphores[i] != NULL) {
      ncNewline();
      ncPrint("Semaphore ID: ");
      ncPrintDec(i);
      ncNewline();
      ncPrint("Current value: ");
      ncPrintDec(semaphores[i]->value);
      ncNewline();
      ncPrint("Processes using this sem: ");
      ncPrintDec(semaphores[i]->activeCount);
      ncNewline();
      ncPrint("Amount of processes blocked: ");
      ncPrintDec(queueSize(semaphores[i]->blockedQueue));
      ncNewline();
      printBlockedProcesses(i);
      ncNewline();
      ncPrint("---------------------------------------");
    }
  }
  ncNewline();
}