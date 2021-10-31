#include <tests.h>

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

void testMM() {
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  while (1) {
    print(1, "Testing memory manager module:\n", 32);
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    print(1, "Requesting blocks.\n", 19);
    while (rq < MAX_BLOCKS && total < MAX_MEMORY) {
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = alloc(mm_rqs[rq].size);
      total += mm_rqs[rq].size;
      rq++;
    }
    // Set
    print(1, "Setting blocks.\n", 16);
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    print(1, "Checking if blocks are valid.\n", 30);
    for (i = 0; i < rq; i++) {
      if (mm_rqs[i].address != NULL) {
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          print(1, "ERROR!\n", 8);
        } else {
          print(1, ". ", 2);
        }
      }
    }
    put_char(1, '\n');

    // Free
    print(1, "Freeing blocks.\n", 16);
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        free(mm_rqs[i].address);

    print(1,
          "Test complete. To stop testing press ENTER or TAB, to continue, "
          "press any other key\n",
          85);
    int c = get_char();
    if (c == '\n' || c == -1) {
      return;
    }
  }
}

// TO BE INCLUDED
void endless_loop(void) {
  while (1)
    ;
}

void testPrs(void) {
  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;

  while (1) {
    put_s(1, "Testing process manager module:\n");

    put_s(1, "Creating processes.\n");
    // Create MAX_PROCESSES processes
    for (rq = 0; rq < MAX_PROCESSES; rq++) {
      p_rqs[rq].pid =
          createPs((uint64_t)&endless_loop, "endless_loop", 0, NULL, 1);

      if (p_rqs[rq].pid == -1) {
        put_s(1, "Error creating process\n");
        return;
      } else {
        p_rqs[rq].state = RUNNING;
        alive++;
      }
      if (exec(p_rqs[rq].pid) < 0) {
        put_s(1, "Error creating process\n");
        return;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been
    // killed
    put_s(1, "Randomply kill, block or unblock process.\n");
    while (alive > 0) {

      for (rq = 0; rq < MAX_PROCESSES; rq++) {
        action = GetUniform(2) % 2;

        switch (action) {
        case 0:
          if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
            if (kill(p_rqs[rq].pid) == -1) {
              put_s(1, "Error killing process\n");
              return;
            }
            p_rqs[rq].state = KILLED;
            alive--;
          }
          break;

        case 1:
          if (p_rqs[rq].state == RUNNING) {
            if (block(p_rqs[rq].pid) == -1) {
              put_s(1, "Error blocking process\n");
              return;
            }
            p_rqs[rq].state = BLOCKED;
          }
          break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < MAX_PROCESSES; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2) {
          if (unblock(p_rqs[rq].pid) == -1) {
            put_s(1, "Error unblocking process\n");
            return;
          }
          p_rqs[rq].state = RUNNING;
        }
      print(1, ". ", 2);
    }
    put_char(1, '\n');
    print(1,
          "Test complete. To stop testing press ENTER or TAB, to continue, "
          "press any other key\n",
          85);
    int c = get_char();
    if (c == '\n' || c == -1) {
      return;
    }
  }
}

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  int64_t aux = *p;
  aux += inc;
  yield();
  *p = aux;
}

void inc(int argc, char *argv[]) {
  int64_t value = atoi(argv[1]);
  uint64_t N = atoi(argv[2]);
  // uint64_t sem, int64_t value, uint64_t N
  uint64_t i;

  if (sem_open(SEM_ID, 1) < 0) {
    put_s(1, "Error opening SEM_ID\n");
    return;
  }
  if (sem_open(SEM_ID2, 1) < 0) {
    put_s(1, "Error opening SEM_ID2\n");
    return;
  }

  for (i = 0; i < N; i++) {
    if (sem_wait(SEM_ID) < 0) {
      put_s(1, "Error in wait SEM_ID\n");
      return;
    }
    slowInc(&global, value);
    if (sem_post(SEM_ID) < 0) {
      put_s(1, "Error in post SEM_ID\n");
      return;
    }
  }
  if (sem_close(SEM_ID) == 0) {
    if (sem_post(SEM_ID2) < 0) {
      put_s(1, "Error in post SEM_ID2\n");
      return;
    }
  }
  sem_close(SEM_ID2);
  print_f(1, "Final value: %d\n", global);
  exit();
}

void testSync() {
  uint64_t i;

  global = 0;

  put_s(1, "Testing sync module...\n");
  if (sem_open(SEM_ID2, 0) < 0) {
    put_s(1, "Error opening SEM_ID2\n");
    return;
  }
  char *argv[4];
  argv[0] = "1";
  argv[1] = "1";
  argv[2] = "10";
  char *argv2[4];
  argv2[0] = "1";
  argv2[1] = "-1";
  argv2[2] = "10";
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    if (exec(createPs((uint64_t)&inc, "inc", 3, argv, 1)) < 0) {
      put_s(1, "Error when executing.\n");
      return;
    } // TODO: add macro foreground / background
    if (exec(createPs((uint64_t)&inc, "inc", 3, argv2, 1)) < 0) {
      put_s(1, "Error when executing.\n");
      return;
    }
  }
  if (sem_wait(SEM_ID2) < 0) {
    put_s(1, "Error in wait SEM_ID2\n");
    return;
  }
  sem_close(SEM_ID2);
  put_s(1, "Test complete.\n");
}

void bussy_wait(uint64_t n) {
  uint64_t i;
  for (i = 0; i < n; i++)
    ;
}

void looping(void) {
  uint64_t pid = getpid();

  while (1) {
    print_f(1, "%d ", pid);
    bussy_wait(MINOR_WAIT);
  }
}

void testPrio(void) {
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  put_s(1, "Testing priorities module...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++) {
    pids[i] = createPs((uint64_t)&looping, "looping", 0, NULL, 1);
    if (exec(pids[i]) < 0) {
      put_s(1, "Error creating process\n");
      return;
    }
  }

  bussy_wait(WAIT);
  put_s(1, "\nChanging priorities...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++) {
    switch (i % 3) {
    case 0:
      nice(pids[i], 0); // lowest priority
      break;
    case 1:
      nice(pids[i], 1); // medium priority
      break;
    case 2:
      nice(pids[i], 2); // highest priority
      break;
    }
  }

  bussy_wait(WAIT);
  put_s(1, "\nBlocking...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    block(pids[i]);

  put_s(1, "Changing priorities while blocked...\n");
  for (i = 0; i < TOTAL_PROCESSES; i++) {
    switch (i % 3) {
    case 0:
      nice(pids[i], 0); // medium priority
      break;
    case 1:
      nice(pids[i], -1); // medium priority
      break;
    case 2:
      nice(pids[i], -2); // medium priority
      break;
    }
  }

  put_s(1, "Unblocking...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    unblock(pids[i]);

  bussy_wait(WAIT);
  put_s(1, "\nKilling...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    kill(pids[i]);

  put_s(1, "Test complete.\n");
}
