// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <my_process.h>

pid_t processManager(uint64_t rip, int argc, char *argv[], uint8_t mode, int new_in, int new_out) {
  pid_t pid = createPs(rip, argc, argv, mode);
  if(pid < 0){
    print_f(STDOUT_FILENO, "Error creating process\n");
    return -1;
  }
  
  if(dup(pid, STDIN_FILENO, new_in) < 0) {
    print_f(STDOUT_FILENO, "Error dupping fd %d\n", STDIN_FILENO);
    return -1;
  }

  if(dup(pid, STDOUT_FILENO, new_out) < 0) {
    print_f(STDOUT_FILENO, "Error dupping fd %d\n", STDOUT_FILENO);
    return -1;
  }

  if(exec(pid) < 0) {
    print_f(STDOUT_FILENO, "Error running process %d\n", pid);
    return -1;
  }

  return pid;
}