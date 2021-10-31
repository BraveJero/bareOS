#include <shellCmds.h>

void helpCmd(void) {
  print_f(STDOUT_FILENO, "Los comandos disponibles son:\n");
  print_f(STDOUT_FILENO, " - help: Muestra los comandos disponibles\n");
  print_f(STDOUT_FILENO, " - inforeg: Muestra el estado de los registros\n");
  print_f(STDOUT_FILENO, " - printmem: Imprime la memoria a partir de la direccion dada\n");
  print_f(STDOUT_FILENO, " - printDate: Imprime informacion acerca del tiempo\n");
  print_f(STDOUT_FILENO, " - printFeatures: Muestra caracteristicas del microprocesador\n");
  print_f(STDOUT_FILENO, " - printQuadraticRoots: Resuelve una funcion cuadratica\n");
  print_f(STDOUT_FILENO, " - invalidOpcode: Genera excepcion por operacion invalida\n");
  print_f(STDOUT_FILENO, " - divisionByZero: Genera excepcion por division por 0\n");
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

void loopCmd(int argc, char *argv[]) {
  int mode = strcmp(argv[argc - 1], "&")? 0 : 1;
  pid_t pid = createPs((uint64_t) &loop, argv[0], argc, argv, mode);
  if(pid < 0){
    print_f(STDOUT_FILENO, "Error creating process\n");
    return;
  }
  if(exec(pid) < 0) {
    print_f(STDOUT_FILENO, "Error running process %d\n", pid);
  }

}
