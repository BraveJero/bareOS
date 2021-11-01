#include <my_process.h>

void processManager(uint64_t rip, int argc, char *argv[], uint8_t mode, int new_in, int new_out) {
  pid_t pid = createPs(rip, argc, argv, mode);
  if(pid < 0){
    print_f(STDOUT_FILENO, "Error creating process\n");
    return;
  }
  dup(pid, STDIN_FILENO, new_in);
  dup(pid, STDOUT_FILENO, new_out);
  if(exec(pid) < 0) {
    print_f(STDOUT_FILENO, "Error running process %d\n", pid);
  }
}