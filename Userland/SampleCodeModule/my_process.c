#include <my_process.h>

void processManager(uint64_t rip, int argc, char *argv[], uint8_t mode, int stdin, int stdout) {
  pid_t pid = createPs(rip, argc, argv, mode);
  if(pid < 0){
    print_f(STDOUT_FILENO, "Error creating process\n");
    return;
  }
  dup(pid, STDIN_FILENO, stdin);
  dup(pid, STDOUT_FILENO, stdout);
  if(exec(pid) < 0) {
    print_f(STDOUT_FILENO, "Error running process %d\n", pid);
  }
}