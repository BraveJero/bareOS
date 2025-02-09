// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <interrupts.h>
#include <kstring.h>
#include <mmgr.h>
#include <naiveConsole.h>
#include <process.h>
#include <scheduler.h>

#define MIN_PRIORITY 0
#define MAX_PRIORITY 9

typedef struct process {
  pid_t pid, parent, wait;
  uint8_t priority, mode;
  Status status;
  uint64_t rsp, rip, stack_base;
  char **argv, *name;
  int fds[2], argc; // save the "stdin" and "stdout" for this process.
} Process;

static pid_t processCounter = 0;
static Process *processes[MAX_PROCESS_COUNT] = {NULL};
static char *states[] = {"Ready", "Terminated", "Blocked"};

static uint8_t isValidPid(pid_t pid) {
  return pid >= 0 && pid < MAX_PROCESS_COUNT && processes[pid] != NULL;
}

pid_t createProcess(uint64_t rip, uint8_t priority, int argc, char *argv[],
                    uint8_t mode) {
  if (processCounter >= MAX_PROCESS_COUNT) {
    return -1;
  }

  Process *newProcess = alloc(sizeof(Process));

  if (newProcess == NULL) {
    return -1;
  }

  newProcess->stack_base = (uint64_t)alloc(PROCESS_SIZE);

  if (newProcess->stack_base == 0) {
    free(newProcess);
    return -1;
  }

  if (argv == NULL) {
    newProcess->argv = NULL;
  } else {
    newProcess->argv = alloc((argc + 1) * sizeof(char *));
    if (newProcess->argv == NULL) {
      free(newProcess);
      return -1;
    }
    for (int i = 0; i < argc; i++) {
      if (argv[i] == NULL) {
        newProcess->argv[i] = NULL;
      } else {
        newProcess->argv[i] = alloc(strlen(argv[i]));
        if (newProcess->argv[i] == NULL) {
          for (int j = 0; j < i; j++) {
            free(newProcess->argv[j]);
          }
          free((void *)newProcess->stack_base);
          free(newProcess);
          return -1;
        }
        strcpy(newProcess->argv[i], argv[i]);
      }
    }
    newProcess->argv[argc] = NULL;
  }

  if (argc > 0 && newProcess->argv && newProcess->argv[0] != NULL) {
    newProcess->name = newProcess->argv[0];
  } else {
    newProcess->name = "";
  }

  priority = priority <= MIN_PRIORITY? MIN_PRIORITY : priority;
  priority = priority > MAX_PRIORITY? MAX_PRIORITY : priority;

  uint64_t rsp = newProcess->stack_base + (PROCESS_SIZE - 1);

  newProcess->rsp =
      init_process(rsp, rip, (uint64_t)argc, (uint64_t)newProcess->argv);
  newProcess->pid = processCounter;
  newProcess->parent = getCurrentPid();
  newProcess->status = READY;
  newProcess->rip = rip;
  newProcess->priority = priority;
  newProcess->mode = mode;
  newProcess->argc = argc;
  newProcess->wait = -1;
  newProcess->fds[READ] = STDIN_FILENO;
  newProcess->fds[WRITE] = STDOUT_FILENO;

  processes[processCounter++] = newProcess;

  return newProcess->pid;
}

int exec(pid_t pid) {
  if (!isValidPid(pid) || isTerminated(pid))
    return -1;

  if (addToReady(processes[pid]->pid) < 0) {
    return -1;
  }

  if (!isBackground(pid)) {
    block(processes[pid]->parent);
  }

  return 0;
}

int kill(pid_t pid) {
  if (!isValidPid(pid) || isTerminated(pid))
    return -1;
  processes[pid]->status = TERMINATED;
  if (!isBackground(pid)) {
    unblock(processes[pid]->parent);
  }

  if(processes[pid]->wait >= 0) {
    unblock(processes[pid]->wait);
  }

  if (pid == getCurrentPid())
    yield_cpu();

  return 0;
}

int block(pid_t pid) {
  if (!isValidPid(pid) || isTerminated(pid))
    return -1;
  processes[pid]->status = BLOCKED;
  if (pid == getCurrentPid())
    yield_cpu();
  return 0;
}

int unblock(pid_t pid) {
  if (!isValidPid(pid) || isTerminated(pid))
    return -1;
  processes[pid]->status = READY;
  return 0;
}

void remove(pid_t pid) {
  if (isValidPid(pid)) {
    free((void *)processes[pid]->stack_base);
    free(processes[pid]);
    if (processes[pid]->argv != NULL) {
      for (int i = 0; processes[pid]->argv[i] != NULL; i++)
        free(processes[pid]->argv[i]);
      free(processes[pid]->argv);
    }
    processes[pid] = NULL;
  }
}

uint64_t getRsp(pid_t pid) { return isValidPid(pid) ? processes[pid]->rsp : 0; }

int getPriority(pid_t pid) {
  if (!isValidPid(pid) || isTerminated(pid))
    return -1;
  return processes[pid]->priority;
}

int8_t isBackground(pid_t pid) { 
  return isValidPid(pid) ? processes[pid]->mode & MASK_BACKGROUND : -1;
}


int getFd(pid_t pid, uint8_t fd) {
  if (!isValidPid(pid) || isTerminated(pid) || fd >= 2)
    return -1;
  return processes[pid]->fds[fd];
}

uint8_t isReady(pid_t pid) {
  return isValidPid(pid) ? processes[pid]->status == READY : 0;
}

uint8_t isBlocked(pid_t pid) {
  return isValidPid(pid) ? processes[pid]->status == BLOCKED : 0;
}

uint8_t isTerminated(pid_t pid) {
  return isValidPid(pid) ? processes[pid]->status == TERMINATED : 1;
}

void setRsp(pid_t pid, uint64_t rsp) {
  if (isValidPid(pid) || isTerminated(pid))
    processes[pid]->rsp = rsp;
}

int setStatusToBlocked(pid_t pid) {
  if (!isValidPid(pid) || isTerminated(pid))
    return -1;
  processes[pid]->status = BLOCKED;
  return 0;
}

int setStatusToReady(pid_t pid) {
  return unblock(pid);
}

int setWaitingPid(pid_t waitFor, pid_t waitOn) {
  if (!isValidPid(waitFor) || isTerminated(waitFor) || !isValidPid(waitOn) || isTerminated(waitOn))
    return -1;
  processes[waitOn]->wait = waitFor;
  return 0;
}

int setPriority(pid_t pid, uint8_t priority) {
  if (!isValidPid(pid) || isTerminated(pid))
    return -1;
  
  priority = priority <= MIN_PRIORITY? MIN_PRIORITY : priority;
  priority = priority > MAX_PRIORITY? MAX_PRIORITY : priority;

  processes[pid]->priority = priority;
  return 0;
}

// changes the stdin/stdout for the given new fd of given process.
int dup(pid_t pid, int old, int new) {
  if ((old != STDIN_FILENO && old != STDOUT_FILENO) || !isValidPid(pid) ||
      isTerminated(pid))
    return -1;
  processes[pid]->fds[old] = new;
  return 0;
}

int wait(pid_t pid) {
  pid_t curr = getCurrentPid();
  if(curr < 0)
    return -1;
  if(setWaitingPid(curr, pid) < 0) {
    return -1;
  }

  return block(curr);
}

void showAllPs() {
  ncNewline();
  ncPrint("---------------------------------------");
  for (int i = 0; i < MAX_PROCESS_COUNT; i++) {
    if (processes[i] != NULL) {
      ncNewline();
      ncPrint("PID: ");
      ncPrintDec(processes[i]->pid);
      ncNewline();
      ncPrint("Parent: ");
      if (processes[i]->parent == -1) {
        ncPrint("-1");
      } else {
        ncPrintDec(processes[i]->parent);
      }
      ncNewline();
      ncPrint("Name: ");
      ncPrint(processes[i]->name);
      ncNewline();
      ncPrint("Current rsp: ");
      ncPrintHex(processes[i]->rsp);
      ncNewline();
      ncPrint("Priority: ");
      ncPrintDec(processes[i]->priority);
      ncNewline();
      ncPrint("Status: ");
      ncPrint(states[processes[i]->status]);
      ncNewline();
      ncPrint("Process reads from: ");
      ncPrintDec(processes[i]->fds[READ]);
      ncNewline();
      ncPrint("Process writes to: ");
      ncPrintDec(processes[i]->fds[WRITE]);
      ncNewline();
      if (isBackground(i)) {
        ncPrint("Process in background");
      } else {
        ncPrint("Process in foreground");
      }
      ncNewline();
      ncPrint("---------------------------------------");
    }
  }
  ncNewline();
}
