/* sampleCodeModule.c */
#include <exceptions.h>
#include <lib.h>
#include <stdint.h>
#include <utils.h>
#include <shellCmds.h>
#include <my_process.h>
#include <string.h>
#include <tests.h>

#define MAX_COMMAND 256 // Habria que achicarlo
#define MAX_ARGS 5
#define MODULES_SIZE 8

#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef void (*commandType)(int argc, char *argv[]);

static char *commandStrings[MODULES_SIZE] = {
    "help",
    "ps",
    "kill",
    "block",
    "unblock",
    "loop",
    "testMM", 
    "testPrs"
    };
static commandType commandFunctions[MODULES_SIZE] = {
    (commandType) helpCmd,
    (commandType) ps,
    (commandType) killCmd,
    (commandType) blockCmd,
    (commandType) unblockCmd,
    (commandType) loopCmd,
    (commandType) testMM,
    (commandType) testPrs
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
  print_f(STDOUT_FILENO, "Ingrese help para ver todos los comandos.\n");
  //exec(createPs((uint64_t) &endlessLoop, "endless loop", 0, NULL, BACKGROUND));

  while (1) {
    print_f(STDERR_FILENO, "\nbareOS $ ");
    int64_t ans = get_s(buffer, MAX_COMMAND);
    if (ans != -1)
      checkModule(buffer);
    else
      print_f(STDOUT_FILENO, "Comando no valido\n");
  }
}

void checkModule(char *string) {
  char *argv[MAX_ARGS] = {NULL};
  int argc = parser(string, argv);
  for (int i = 0; i < MODULES_SIZE; i++) {
    if (!strcmp(argv[0], commandStrings[i])) {
      commandFunctions[i](argc, argv);
      return;
    }
  }
  print_f(STDOUT, "Comando no valido\n");
}
