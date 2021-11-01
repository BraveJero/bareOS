#include <shellCmds.h>

void helpCmd(void) {
  print_f(STDOUT_FILENO, "Commands:\n");
  print_f(STDOUT_FILENO, " - help: Displays a list of all available commands.\n");
  print_f(STDOUT_FILENO, " - memDump: Displays information about the heap.\n");
  print_f(STDOUT_FILENO, " - ps: Displays a list of all the current processes.\n");
  print_f(STDOUT_FILENO, " - kill <pid>: Kills the process of pid <pid>.\n");
  print_f(STDOUT_FILENO, " - nice <pid> <step>: Changes the priority of process <pid> by <step> steps (it can be negative).\n");
  print_f(STDOUT_FILENO, " - block <pid>: Blocks the process of pid <pid>.\n");
  print_f(STDOUT_FILENO, " - sem: Displays a list of all open Semaphores.\n");
  print_f(STDOUT_FILENO, " - cat: Print on the standar output.\n");
  print_f(STDOUT_FILENO, " - wc: Print byte and newline counts for standar input.\n");
  print_f(STDOUT_FILENO, " - filter: Filters the vowels out of the standar input.\n");
  print_f(STDOUT_FILENO, " - pipe: Displays a list of all open pipes.\n");
  print_f(STDOUT_FILENO, " - phylo: Implements the Dining Philosophers.\n");
  print_f(STDOUT_FILENO, " - loop: Prints pid, argc and argv after a given amount of time.\n");
  print_f(STDOUT_FILENO, " - testPrio: Tests priority.\n");
  print_f(STDOUT_FILENO, " - testSync: Tests synchronization.\n");
  print_f(STDOUT_FILENO, " - testMM: Tests memory manager.\n");
  print_f(STDOUT_FILENO, " - testPrs: tests process creation.\n");

}

void killCmd(int argc, char *argv[]) {
  if(argc != 2) {
    print_f(STDOUT_FILENO, "Invalid amount of arguments\n");
    return;
  }
  pid_t pid = atoi(argv[STDOUT_FILENO]);
  if(pid == 0) {
    print_f(STDOUT_FILENO, "Invalid pid\n");
    return;
  }

  if(kill(pid) < 0) {
    print_f(STDOUT_FILENO, "Error killing process %d\n", pid);
  }
}

void blockCmd(int argc, char *argv[]){
  if(argc != 2) {
    print_f(STDOUT_FILENO, "Invalid amount of arguments\n");
    return;
  }
  pid_t pid = atoi(argv[STDOUT_FILENO]);
  if(pid == 0) {
    print_f(STDOUT_FILENO, "Invalid pid\n");
    return;
  }

  if(block(pid) < 0) {
    print_f(STDOUT_FILENO, "Error blocking process %d\n", pid);
  }
}

void unblockCmd(int argc, char *argv[]){
  if(argc != 2) {
    print_f(STDOUT_FILENO, "Invalid amount of arguments\n");
    return;
  }
  pid_t pid = atoi(argv[STDOUT_FILENO]);
  if(pid == 0) {
    print_f(STDOUT_FILENO, "Invalid pid\n");
    return;
  }

  if(unblock(pid) < 0) {
    print_f(STDOUT_FILENO, "Error unblocking process %d\n", pid);
  }
}

void niceCmd(int argc, char *argv[]){
  if(argc != 3) {
    print_f(STDOUT_FILENO, "Invalid amount of arguments\n");
    return;
  }
  pid_t pid = atoi(argv[1]);
  if(pid == 0) {
    print_f(STDOUT_FILENO, "Invalid pid\n");
    return;
  }

  int adj = atoi(argv[2]);

  if(nice(pid, adj) < 0) {
    print_f(STDOUT_FILENO, "Error unblocking process %d\n", pid);
  }
}

void loop(int argc, char *argv[]) {
  print_f(STDOUT_FILENO, "I'm process with pid %d\n", getpid());
  print_f(STDOUT_FILENO, "I have %d arguments \n", argc);
  print_f(STDOUT_FILENO, "My arguments are: ");
  for(int i = 0; i < argc; i++) {
    print_f(STDOUT_FILENO, "%s ", argv[i]);
  }
  put_char(STDOUT_FILENO, '\n');
  exit();
}

void loopCmd(int argc, char *argv[], int mode, int new_in, int new_out) {
  processManager((uint64_t) &loop, argc, argv, mode, new_in, new_out);
}

void catCmd(int argc, char *argv[], int mode, int new_in, int new_out) {
  processManager((uint64_t) &cat, argc, argv, mode, new_in, new_out);
}

void filterCmd(int argc, char *argv[], int mode, int new_in, int new_out) {
  processManager((uint64_t) &filter, argc, argv, mode, new_in, new_out);
}

void wcCmd(int argc, char *argv[], int mode, int new_in, int new_out) {
  processManager((uint64_t) &wc, argc, argv, mode, new_in, new_out);
}