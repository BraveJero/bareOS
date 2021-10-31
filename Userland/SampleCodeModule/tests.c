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
    print(1, "Requesting blocks\n", 19);
    while (rq < MAX_BLOCKS && total < MAX_MEMORY) {
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address =
          alloc(mm_rqs[rq].size); // TODO: Port this call as required
      total += mm_rqs[rq].size;
      rq++;
    }
    // Set
    print(1, "Setting blocks\n", 16);
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        memset(mm_rqs[i].address, i,
               mm_rqs[i].size); // TODO: Port this call as required

    // Check
    print(1, "Checking if blocks are valid\n", 30);
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
          print(1, "ERROR!\n", 8); // TODO: Port this call as required
        else
          print(1, ". ", 2);

    put_char(1, '\n');

    // Free
    print(1, "Freeing blocks\n", 16);
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        free(mm_rqs[i].address); // TODO: Port this call as required

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

    // Create MAX_PROCESSES processes
    for (rq = 0; rq < MAX_PROCESSES; rq++) {
      p_rqs[rq].pid = createPs(&endless_loop, "endless_loop", 0, NULL,
                               1); // TODO: Port this call as required

      if (p_rqs[rq].pid == -1) {              // TODO: Port this as required
        put_s(1, "Error creating process\n"); // TODO: Port this as required
        return;
      } else {
        p_rqs[rq].state = RUNNING;
        alive++;
      }
      exec(p_rqs[rq].pid);
    }

    // Randomly kills, blocks or unblocks processes until every one has been
    // killed
    while (alive > 0) {

      for (rq = 0; rq < MAX_PROCESSES; rq++) {
        action = GetUniform(2) % 2;

        switch (action) {
        case 0:
          if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
            if (kill(p_rqs[rq].pid) == -1) { // TODO: Port this as required
              put_s(1,
                    "Error killing process\n"); // TODO: Port this as required
              return;
            }
            p_rqs[rq].state = KILLED;
            alive--;
          }
          break;

        case 1:
          if (p_rqs[rq].state == RUNNING) {
            if (block(p_rqs[rq].pid) == -1) { // TODO: Port this as required
              put_s(1,
                    "Error blocking process\n"); // TODO: Port this as required
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
          if (unblock(p_rqs[rq].pid) == -1) { // TODO: Port this as required
            put_s(1,
                  "Error unblocking process\n"); // TODO: Port this as required
            return;
          }
          p_rqs[rq].state = RUNNING;
        }

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
}
