// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <philo.h>

#define INITIAL_SEM_ID 60
#define INITIAL_PHIL_NUM 5
#define MIN_PHIL_NUM 2
#define MAX_PHIL_NUM 9

typedef enum {Thinking = 0, Hungry, Eating} State;

#define CLEAN_BUF while(get_char() != '\n');

static pid_t philosophersPids[MAX_PHIL_NUM] = {0};
static State states[MAX_PHIL_NUM] = {Thinking};
static uint8_t coordinate[MAX_PHIL_NUM] = {0}, lock;
static int count = 0;

void eat(int id);
void think(int id);
int getLeftIndex(int id);
int getRightIndex(int id);
void createPhilosophers(int amount);
void killPhilosophers(int amount);
void openSems(int amount);
void closeSems(int amount);
void test(int id);
void takeForks(int id);
void putForks(int id);
void printState(void);


void philosopher(int argc, char *argv[]) {
  int id = atoi(argv[1]);
  while(1) {
    think(id);
    takeForks(id);
    eat(id);
    putForks(id);
  }
  exit();
}

void philoController(int argc, char *argv[]) {
  count = INITIAL_PHIL_NUM;
  createPhilosophers(count);
  openSems(count);
  while(1);
  killPhilosophers(count);
  closeSems(count);
  exit();
}

void eat(int id) {
  //print_f(STDOUT_FILENO, "Philosopher %d eating... \n", id);
  sleep(id % 3 + 1);
}

void think(int id) {
  //print_f(STDOUT_FILENO, "Philosopher %d thinking... \n", id);
  sleep((id + 1) % 3+ 1);
}

int getLeftIndex(int id) {
  return (id + count - 1) % count;
}

int getRightIndex(int id) {
  return (id + 1) % count;
}

void createPhilosophers(int amount) {
  char id[2];
  id[1] = '\0';
  char *argv[] = {"philosopher", id, NULL};
  for(int i = 0; i < amount; i++) {
    id[0] = '0' + i;
    pid_t pid;
    if((pid = createPs((uint64_t) &philosopher, 2, argv, BACKGROUND)) < 0) {
      killPhilosophers(amount);
      print_f(STDERR_FILENO, "Error creating philosophers\n");
    }
    philosophersPids[i] = pid;
    if(exec(pid) < 0) {
      killPhilosophers(amount);
      print_f(STDERR_FILENO, "Error running philosophers\n");
    }
  }
}

void killPhilosophers(int amount) {
  for(int i = 0; i < amount; i++)
    kill(philosophersPids[i]);
}

void openSems(int amount) {
  int i;
  for(i = 0; i < count; i++) {
    if(sem_open(INITIAL_SEM_ID + i, 0) < 0) {
      killPhilosophers(amount);
      print_f(STDERR_FILENO, "Error opening sems. \n");
      exit();
    }
    coordinate[i] = INITIAL_SEM_ID + i;
  }
  sem_open(i + INITIAL_SEM_ID, 1);
  lock = i + INITIAL_SEM_ID;
}

void closeSems(int amount) {
  for(int i = 0; i < count; i++) {
    sem_close(coordinate[i]);
  }
  sem_close(lock);
}

void test(int id) {
  if(states[id] == Hungry &&
    states[getLeftIndex(id)] != Eating &&
    states[getRightIndex(id)] != Eating) {
    states[id] = Eating;
    printState();
    sem_post(coordinate[id]);
  }
}

void takeForks(int id) {
  sem_wait(lock);
  states[id] = Hungry;
  test(id);
  sem_post(id);
  sem_post(lock);
  sem_wait(coordinate[id]); // Block if I'm not eating.
}

void putForks(int id) {
  sem_wait(lock);
  states[id] = Thinking;
  test(getLeftIndex(id));
  test(getRightIndex(id));
  sem_post(lock);
}

void printState(void) {
  for(int i = 0; i < count; i++){
    print_f(STDOUT_FILENO, " %c ", states[i] == Eating ? 'E' : '.');
  }
  put_char(STDOUT_FILENO, '\n');
}