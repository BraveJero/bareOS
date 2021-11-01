/* sampleCodeModule.c */
#include <exceptions.h>
#include <input.h>
#include <lib.h>
#include <my_mem.h>
#include <my_pipes.h>
#include <my_process.h>
#include <my_sync.h>
#include <my_video.h>
#include <shellCmds.h>
#include <stdint.h>
#include <string.h>
#include <tests.h>
#include <utils.h>

#define MODULES_SIZE 19

typedef void (*commandType)(int argc, char *argv[], int mode, int, int);

static char *commandStrings[MODULES_SIZE] = {
    "help",
    "ps",
    "kill",
    "block",
    "unblock",
    "nice",
    "loop",
    "cat",
    "filter",
    "wc",
    "testMM", 
    "testPrs",
    "testPrio",
    "testSync",
    "pipe",
    "sem",
    "memDump",
    "clear",
    "bikegoesbrrr"
    };
static commandType commandFunctions[MODULES_SIZE] = {
    (commandType) helpCmd,
    (commandType) ps,
    (commandType) killCmd,
    (commandType) blockCmd,
    (commandType) unblockCmd,
    (commandType) niceCmd,
    (commandType) loopCmd,
    (commandType) catCmd,
    (commandType) filterCmd,
    (commandType) wcCmd,
    (commandType) testMM,
    (commandType) testPrs,
    (commandType) testPrio,
    (commandType) testSync,
    (commandType) pipe_dump,
    (commandType) sem_dump,
    (commandType) mem_dump,
    (commandType) clear,
    (commandType) broombroom,
    };

void checkModule(char *string, int, int);
void getCommand(char *str);

void endlessLoop(void) {
  for (uint64_t i = 0; 1; i++) {
    if (i % 5000000 == 0) {
      print_f(STDOUT_FILENO, "A");
    }
  }
}

int main() {
  char buffer[MAX_COMMAND + 1];
  print_f(STDOUT_FILENO, "Welcome to...\n\n");
  
  put_s(STDOUT_FILENO, "'########:::::'###::::'########::'########::'#######:::'######::'####:\n##.... ##:::'## ##::: ##.... ##: ##.....::'##.... ##:'##... ##: ####:\n##:::: ##::'##:. ##:: ##:::: ##: ##::::::: ##:::: ##: ##:::..:: ####:\n########::'##:::. ##: ########:: ######::: ##:::: ##:. ######::: ##::\n##.... ##: #########: ##.. ##::: ##...:::: ##:::: ##::..... ##::..:::\n##:::: ##: ##.... ##: ##::. ##:: ##::::::: ##:::: ##:'##::: ##:'####:\n########:: ##:::: ##: ##:::. ##: ########:. #######::. ######:: ####:\n.......:::..:::::..::..:::::..::........:::.......::::......:::....::");
  
  print_f(STDOUT_FILENO, "\n\nType 'help' to see a list of all available commands.\n");

  while (1) {
    print_f(STDERR_FILENO, "\nbareOS $ ");
    int64_t ans = get_s(buffer, MAX_COMMAND);
    if (ans != -1) {
      if (buffer[ans - 1] == '\n')
        buffer[ans - 1] = 0;
      else
        put_char(STDOUT_FILENO, '\n');
      getCommand(buffer);
    } else
      print_f(STDOUT_FILENO, "Invalid Command\n");
  }
}

void checkModule(char *string, int new_in, int new_out) {
  char *argv[MAX_ARGS] = {NULL};
  int argc = parser(string, argv);
  int idx = findCmd(argv[0], commandStrings, MODULES_SIZE);
  if (idx < 0)
    print_f(STDOUT_FILENO, "%s: Command not found \n", string);
  else {
    int mode;
    if (argv[argc - 1][0] == '&')
      mode = BACKGROUND;
    else
      mode = FOREGROUND;
    commandFunctions[idx](argc, argv, mode, new_in, new_out);
  }
}

void getCommand(char *str) {
  char *p = strchr(str, '|');
  if (p == NULL)
    return checkModule(str, STDIN_FILENO, STDOUT_FILENO);
  int fds[2], id;
  if ((id = pipe(-1, fds)) < 0) { // First available pipe.
    print_f(STDOUT_FILENO, "Error opening pipe \n");
    return;
  }
  *p = '\0';
  char *cmd1 = str, *cmd2 = p + 1;

  checkModule(cmd1, STDIN_FILENO, fds[1]);
  plugPipe(id);
  checkModule(cmd2, fds[0], STDOUT_FILENO);

  closePipe(id);
}
