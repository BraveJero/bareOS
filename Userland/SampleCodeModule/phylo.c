// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <phylo.h>

static int getLeft(int index) {
  if (index == 0) {
    return state[active - 1];
  }
  return state[index - 1];
}

static int getRight(int index) {
  if (index == active - 1) {
    return state[0];
  }
  return state[index + 1];
}

void phylosopher(int argc, char *argv[]) {
  uint8_t index = atoi(argv[1]);
  uint16_t mutex = sem_open(SEM_ID, 1);
  if (mutex < 0) {
    print_f(STDERR_FILENO, "Error opening semaphore - SEM_ID: '%d'\n", SEM_ID);
    exit();
  }
  while (active) {
    if (sem_wait(mutex) < 0) {
      print_f(STDERR_FILENO, "Error waiting semaphore - SEM_ID: '%d'\n",
              SEM_ID);
      exit();
    }
    if (state[index] == THINKING) {
      state[index] = HUNGRY;
      if (getLeft(index) != EATING && getLeft(index) != EATING) {
        state[index] = EATING;
      }
      if (sem_wait(mutex) < 0) {
      print_f(STDERR_FILENO, "Error waiting semaphore - SEM_ID: '%d'\n",
              SEM_ID);
      exit();
    }
    } else {
      
    }

    if (sem_post(mutex) < 0) {
      print_f(STDERR_FILENO, "Error posting semaphore - SEM_ID: '%d'\n",
              SEM_ID);
      exit();
    }
  }
  if (sem_close(mutex) < 0) {
    print_f(STDERR_FILENO, "Error closing semaphore - SEM_ID: '%d'\n", SEM_ID);
  }
  exit();
}

void phylo(int argc, char *argv[]) {
  uint32_t phylos = 2, i = 0;
  if (argc >= 2)
    phylos = atoi(argv[1]);
  if (phylos < 2 || phylos > MAX_FILOS) {
    print_f(STDERR_FILENO, "'%d' is not a valid argument\n", phylos);
    exit();
  }
  uint16_t mutex = sem_open(SEM_ID, 0);
  if (mutex < 0) {
    print_f(STDERR_FILENO, "Error opening semaphore - SEM_ID: '%d'\n", SEM_ID);
    exit();
  }

  active = phylos;

  while (i < phylos) {
    char *argv[2];
    argv[0] = "phylosopher";
    argv[1] = alloc(sizeof(char) * 2);
    argv[1][0] = '0' + i;
    argv[1][1] = '\0';
    phylosophers[i++] = createPs(&phylosopher, 2, argv, BACKGROUND);
    sems[i - 1] = sem_open(i + 50, 0);
    if (phylosophers[i - 1] < 0 || sems[i - 1] < 0) {
      if (sems[i - 1] < 0 && phylosophers[i - 1] >= 0)
        kill(phylosophers[i]);
      while (--i > 0) {
        sem_close(sems[i]);
        kill(phylosophers[i]);
      }
      exit();
    }
    exec(phylosophers[i - 1]);
    free(argv[1]);
  }

  if (sem_post(mutex) < 0) {
    print_f(STDERR_FILENO, "Error posting semaphore - SEM_ID: '%d'\n", SEM_ID);
    exit();
  }

  int c = 0;
  do {

    if (c != 0)
      CLEAN_BUF;
  } while ((c = get_char()) != EOF);
  active = 0;
  exit();
}