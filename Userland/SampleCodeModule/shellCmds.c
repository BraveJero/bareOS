// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shellCmds.h>

void help(void) {
  print_f(STDOUT_FILENO, "Commands:\n");
  print_f(STDOUT_FILENO,
          " - help: Displays a list of all available commands.\n");
  print_f(STDOUT_FILENO, " - memDump: Displays information about the heap.\n");
  print_f(STDOUT_FILENO,
          " - ps: Displays a list of all the current processes.\n");
  print_f(STDOUT_FILENO, " - kill <pid>: Kills the process of pid <pid>.\n");
  print_f(STDOUT_FILENO,
          " - nice <pid> <step>: Changes the priority of process <pid> by "
          "<step> steps (it can be negative).\n");
  print_f(STDOUT_FILENO, " - block <pid>: Blocks the process of pid <pid>.\n");
  print_f(STDOUT_FILENO, " - sem: Displays a list of all open Semaphores.\n");
  print_f(STDOUT_FILENO, " - cat: Print on the standar output.\n");
  print_f(STDOUT_FILENO,
          " - wc: Print byte and newline counts for standar input.\n");
  print_f(STDOUT_FILENO,
          " - filter: Filters the vowels out of the standar input.\n");
  print_f(STDOUT_FILENO, " - pipe: Displays a list of all open pipes.\n");
  print_f(STDOUT_FILENO, " - phylo: Implements the Dining Philosophers.\n");
  print_f(STDOUT_FILENO, " - loop: Prints pid once every timer tick.\n");
  print_f(STDOUT_FILENO,
          " - loop: Prints pid once every timer tick. It ALWAYS runs in background\n");
  print_f(STDOUT_FILENO, " - printArgs <argv>: Prints argc and argv.\n");
  print_f(STDOUT_FILENO, " - clear: Clears the screen.\n\n");
  print_f(STDOUT_FILENO, "TESTS:\n - testPrio: Tests priority.\n");
  print_f(STDOUT_FILENO, " - testSync: Tests synchronization.\n");
  print_f(STDOUT_FILENO, " - testNoSync: Tests without synchronization.\n");
  print_f(STDOUT_FILENO, " - testMM: Tests memory manager.\n");
  print_f(STDOUT_FILENO, " - testPrs: Tests process creation.\n\n");
  print_f(STDOUT_FILENO, "EOF is signaled by TAB.\n");
  exit();
}

pid_t helpCmd(int argc, char *argv[], int mode, int new_in, int new_out) {
  return processManager((uint64_t)&help, argc, argv, mode, new_in, new_out);
}

void killCmd(int argc, char *argv[]) {
  if (argc != 2) {
    print_f(STDOUT_FILENO, "Invalid amount of arguments\n");
    return;
  }
  pid_t pid = atoi(argv[STDOUT_FILENO]);
  if (pid == 0) {
    print_f(STDOUT_FILENO, "Invalid pid\n");
    return;
  }

  if (kill(pid) < 0) {
    print_f(STDOUT_FILENO, "Error killing process %d\n", pid);
    return;
  }
  print_f(STDOUT_FILENO, "Process %d killed succesfully. \n", pid);
}

void blockCmd(int argc, char *argv[]) {
  if (argc != 2) {
    print_f(STDOUT_FILENO, "Invalid amount of arguments\n");
    return;
  }
  pid_t pid = atoi(argv[STDOUT_FILENO]);
  if (pid == 0) {
    print_f(STDOUT_FILENO, "Invalid pid\n");
    return;
  }

  if (block(pid) < 0) {
    print_f(STDOUT_FILENO, "Error blocking process %d\n", pid);
    return;
  }
  print_f(STDOUT_FILENO, "Process %d blocked succesfully. \n", pid);
}

void unblockCmd(int argc, char *argv[]) {
  if (argc != 2) {
    print_f(STDOUT_FILENO, "Invalid amount of arguments\n");
    return;
  }
  pid_t pid = atoi(argv[STDOUT_FILENO]);
  if (pid == 0) {
    print_f(STDOUT_FILENO, "Invalid pid\n");
    return;
  }

  if (unblock(pid) < 0) {
    print_f(STDOUT_FILENO, "Error unblocking process %d\n", pid);
    return;
  }
  print_f(STDOUT_FILENO, "Process %d unblocked succesfully. \n", pid);
}

void niceCmd(int argc, char *argv[]) {
  if (argc != 3) {
    print_f(STDOUT_FILENO, "Invalid amount of arguments\n");
    return;
  }
  pid_t pid = atoi(argv[1]);
  if (pid == 0) {
    print_f(STDOUT_FILENO, "Invalid pid\n");
    return;
  }

  int adj = atoi(argv[2]);

  if (nice(pid, adj) < 0) {
    print_f(STDOUT_FILENO, "Error modifying process %d priority\n", pid);
    return;
  }
  print_f(STDOUT_FILENO, "Process %d had it priority succesfully adjusted. \n",
          pid);
}

void loop(int argc, char *argv[]) {
  void *ptr = alloc(2000);
  while (1) {
    uint64_t ticks = getticks();
    print_f(STDOUT_FILENO, " %d ", getpid());
    while (ticks == getticks())
      ; // Print once for every timer tick;
  }
  exit();
}

pid_t loopCmd(int argc, char *argv[], int mode, int new_in, int new_out) {
  return processManager((uint64_t)&loop, argc, argv, BACKGROUND, new_in,
                        new_out);
}

void printArgs(int argc, char *argv[]) {
  print_f(STDOUT_FILENO, "I'm process with pid %d.\n");
  print_f(STDOUT_FILENO, "I have %d arguments.\n", argc);
  print_f(STDOUT_FILENO, "My arguments are: ");
  for (int i = 0; i < argc && argv[i] != NULL; i++)
    print_f(STDOUT_FILENO, "%s ", argv[i]);
  put_char(STDOUT_FILENO, '\n');
  exit();
}

pid_t printArgsCmd(int argc, char *argv[], int mode, int new_in, int new_out) {
  return processManager((uint64_t)&printArgs, argc, argv, mode, new_in,
                        new_out);
}

pid_t catCmd(int argc, char *argv[], int mode, int new_in, int new_out) {
  return processManager((uint64_t)&cat, argc, argv, mode, new_in, new_out);
}

pid_t filterCmd(int argc, char *argv[], int mode, int new_in, int new_out) {
  return processManager((uint64_t)&filter, argc, argv, mode, new_in, new_out);
}

pid_t wcCmd(int argc, char *argv[], int mode, int new_in, int new_out) {
  return processManager((uint64_t)&wc, argc, argv, mode, new_in, new_out);
}

pid_t philoCmd(int argc, char *argv[], int mode, int new_in, int new_out) {
  return processManager((uint64_t)&philoController, argc, argv, mode, new_in,
                        new_out);
}

void broombroom(void) {
  put_s(STDOUT_FILENO,
        "                         _\n                       ,S/  .e.##ee\n     "
        "                ,SS/_ /#####\"\"\n                   ,SSSSSS`|##|\n   "
        "              ,'|SSSSSS/%%##|\n                 | ;SSSSS/%%%%%%/ "
        ".-\"\"-._                           __..ooo._.sSSSSSSSSS\"7.\n        "
        "         |/SSSSS/%%%%%%/.'       `._ __               "
        "_.od888888888888\"'  '\"SSSSS\"\n             ___  "
        "`\"SSS/%%%%%%/\"-.,sSSs._   8888o._    __.o888888888"
        "SS"
        "    `-._    `7Sb\n      _.sssSSSSSSSSSSS/`%%%%/ "
        ",sSSSSSSSSS-.888888888888888888888\"'.S\"         ,SSS\"\"   `7b\n   "
        ",+\"\"       ```\"\"SS/%%%%./,sSSSSSSSS\".   "
        "`\"888888888888888\"'.sSS\"         ,SS\"'        `S.\n               "
        "     /%%%%%%/sSSSSSSSS\"   `s.   `\"88888888\"'.sSSSS\"         ,S\"' "
        "            7\n                   /%%%%%%/ `SSSSSSS$$,..sSS`-.   "
        "`\"88'.sSSSSSSSS._     ,-'\n                  /%%%%%%/    "
        "`SSSS$$$$SSS\",\\\\\\`-.   `\"SSSSSS\"  8\"\"\"7.-'\n                 "
        " /`%%/      `SS$$$SSS,dP,s.\\\\//`-.   `SS\" ,'`8       "
        ",ee888888ee.\n        ,oo8888oo/ /         "
        "`\"\"\"\",d88Pd8888./,-'/`.  `,-._`d'    ,d88888888888888b.\n     "
        ",d888888888/ /8b.          d8888Pd8888888bd88b`.  :_._`8   ,888888\"' "
        "   '\"\"88888.\n   ,888P'      / /\"888b.       d88888`8888888Pd8888 "
        ":  :_`-.( ,d8888.__           7888b.\n  d88P        / /   `788b     "
        "(88888:. `8888Pd88888 ;  ; `-._ 8888P_Z_.>-"
        "--.._   `8888\n d88'     ,--/ /      `88b     `88888b`. `8P 78888\";  "
        ";      `\"*88_,\"   s88s.       `888b\nd88'    ,',$/ /$$$$.   `88b    "
        "  `8888b `. `\"'88\"_/_,'`-._         `-.d8\"88\"8P.      `888.\n888  "
        "  ; ,$$$$$$$$$'    888        `\"8'   `---------------`-.._      "
        "8888888888       888'\n888    : `$$$$$$$':     888                    "
        "             '888b`-._8s888888\"P      .888'\n788.   `  `$$$$'  ;     "
        "88P                                  8888.   \"8878888P'      d888\n "
        "88b    `.  `\"' ,'     d88'                                  '888b    "
        " '88s8\"'     .d888'\n `88b.    `-..-'      d88'                      "
        "              '888b.             .dd888'\n   788b.            ,888'   "
        "                                    7888b._       _.d8888P\n    "
        "`7888oo..__..oo888'                                          "
        "`\"8888888888888888\"'\n      `\"7888888888P\"'                       "
        "                        `\"788 mGk 8P\"'\"\n");
}