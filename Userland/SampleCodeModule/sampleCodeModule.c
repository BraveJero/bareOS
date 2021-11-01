/* sampleCodeModule.c */
#include <exceptions.h>
#include <lib.h>
#include <stdint.h>
#include <utils.h>
#include <shellCmds.h>
#include <my_process.h>
#include <my_pipes.h>
#include <my_mem.h>
#include <my_sync.h>
#include <string.h>
#include <tests.h>
#include <input.h>

#define MODULES_SIZE 15

typedef void (*commandType)(int argc, char *argv[], int mode, int new_stdin, int new_stdout);

static char *commandStrings[MODULES_SIZE] = {
    "help",
    "ps",
    "kill",
    "block",
    "unblock",
    "loop",
    "cat",
    "testMM", 
    "testPrs",
    "testPrio",
    "testSync",
    "pipe",
    "sem",
    "memDump"
    };
static commandType commandFunctions[MODULES_SIZE] = {
    (commandType) helpCmd,
    (commandType) ps,
    (commandType) killCmd,
    (commandType) blockCmd,
    (commandType) unblockCmd,
    (commandType) loopCmd,
    (commandType) catCmd,
    (commandType) testMM,
    (commandType) testPrs,
    (commandType) testPrio,
    (commandType) testSync,
    (commandType) pipe_dump,
    (commandType) sem_dump,
    (commandType) mem_dump
    };

void checkModule(char *string);

void endlessLoop(void) {
  for(uint64_t i  = 0; 1; i++) {
    if(i % 5000000 == 0) {
      print_f(STDOUT_FILENO, "A");
    }
  }
}

int main() {
  char buffer[MAX_COMMAND + 1];
  print_f(STDOUT_FILENO, "Welcome to bareOS!\n\nType 'help' to see a list of all available commands.\n");
  //exec(createPs((uint64_t) &endlessLoop, "endless loop", 0, NULL, BACKGROUND));

  while (1) {
    print_f(STDERR_FILENO, "\nbareOS $ ");
    int64_t ans = get_s(buffer, MAX_COMMAND);
    if (ans != -1)
      checkModule(buffer);
    else
      print_f(STDOUT_FILENO, "Invalid Command\n");
  }
}

void checkModule(char *string) {
  char *argv[MAX_ARGS] = {NULL};
  int argc = parser(string, argv);
  for (int i = 0; i < MODULES_SIZE; i++) {
    if (!strcmp(argv[0], commandStrings[i])) {
      int mode = strcmp(argv[argc - 1], "&") == 0;
      if(mode) {
        argc--;
        argv[argc] = NULL;
      }
      commandFunctions[i](argc, argv, mode, STDIN_FILENO, STDOUT_FILENO);
      return;
    }
  }
  print_f(STDOUT_FILENO, "Invalid Command\n");
}
