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

void broombroom(void) {
  put_s(STDOUT_FILENO, "                         _\n                       ,S/  .e.##ee\n                     ,SS/_ /#####\"\"\n                   ,SSSSSS`|##|\n                 ,'|SSSSSS/%%##|\n                 | ;SSSSS/%%%%%%/ .-\"\"-._                           __..ooo._.sSSSSSSSSS\"7.\n                 |/SSSSS/%%%%%%/.'       `._ __               _.od888888888888\"'  '\"SSSSS\"\n             ___  `\"SSS/%%%%%%/\"-.,sSSs._   8888o._    __.o888888888""SS""    `-._    `7Sb\n      _.sssSSSSSSSSSSS/`%%%%/ ,sSSSSSSSSS-.888888888888888888888\"'.S\"         ,SSS\"\"   `7b\n   ,+\"\"       ```\"\"SS/%%%%./,sSSSSSSSS\".   `\"888888888888888\"'.sSS\"         ,SS\"'        `S.\n                    /%%%%%%/sSSSSSSSS\"   `s.   `\"88888888\"'.sSSSS\"         ,S\"'             7\n                   /%%%%%%/ `SSSSSSS$$,..sSS`-.   `\"88'.sSSSSSSSS._     ,-'\n                  /%%%%%%/    `SSSS$$$$SSS\",\\\\\\`-.   `\"SSSSSS\"  8\"\"\"7.-'\n                  /`%%/      `SS$$$SSS,dP,s.\\\\//`-.   `SS\" ,'`8       ,ee888888ee.\n        ,oo8888oo/ /         `\"\"\"\",d88Pd8888./,-'/`.  `,-._`d'    ,d88888888888888b.\n     ,d888888888/ /8b.          d8888Pd8888888bd88b`.  :_._`8   ,888888\"'    '\"\"88888.\n   ,888P'      / /\"888b.       d88888`8888888Pd8888 :  :_`-.( ,d8888.__           7888b.\n  d88P        / /   `788b     (88888:. `8888Pd88888 ;  ; `-._ 8888P_Z_.>-""--.._   `8888\n d88'     ,--/ /      `88b     `88888b`. `8P 78888\";  ;      `\"*88_,\"   s88s.       `888b\nd88'    ,',$/ /$$$$.   `88b      `8888b `. `\"'88\"_/_,'`-._         `-.d8\"88\"8P.      `888.\n888    ; ,$$$$$$$$$'    888        `\"8'   `---------------`-.._      8888888888       888'\n888    : `$$$$$$$':     888                                 '888b`-._8s888888\"P      .888'\n788.   `  `$$$$'  ;     88P                                  8888.   \"8878888P'      d888\n 88b    `.  `\"' ,'     d88'                                  '888b     '88s8\"'     .d888'\n `88b.    `-..-'      d88'                                    '888b.             .dd888'\n   788b.            ,888'                                       7888b._       _.d8888P\n    `7888oo..__..oo888'                                          `\"8888888888888888\"'\n      `\"7888888888P\"'                                               `\"788 mGk 8P\"'\"\n");
}

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
  // exec(createPs((uint64_t) &endlessLoop, 0, NULL, BACKGROUND));

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
