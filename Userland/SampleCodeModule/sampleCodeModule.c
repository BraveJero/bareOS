/* sampleCodeModule.c */
#include <exceptions.h>
#include <lib.h>
#include <stdint.h>
#include <utils.h>
#include <my_process.h>
#include <string.h>
#include <tests.h>

#define MAX_COMMAND 256 // Habria que achicarlo
#define MAX_ARGS 5
#define MODULES_SIZE 9

#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef void (*commandType)(int argc, char *argv[]);

static char *commandStrings[MODULES_SIZE] = {
    "help",           "inforeg",       "printmem",      "printDate",
    "divisionByZero", "invalidOpcode", "printFeatures", "printQuadraticRoots",
    "testMM"};
static commandType commandFunctions[MODULES_SIZE] = {
    help,
    inforeg,
    printmem,
    printDate,
    throwDivisionByZeroException,
    throwInvalidOpcodeException,
    printFeatures,
    printQuadraticRoots,
    testMM};

void checkModule(char *string);

int main() {
  char buffer[MAX_COMMAND + 1];
  print_f(1, "Ingrese help para ver todos los comandos.\n");

  while (1) {
    print_f(2, "\nbareOS $> ");
    int64_t ans = get_s(buffer, MAX_COMMAND);
    if (ans != -1)
      checkModule(buffer);
    else
      print_f(1, "Comando no valido\n");
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
  print_f(1, "Comando no valido\n");
}
