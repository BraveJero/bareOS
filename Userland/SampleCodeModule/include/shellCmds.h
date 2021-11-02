#ifndef __SHELL_CMDS_H__
#define __SHELL_CMDS_H__

#include <utils.h>
#include <my_process.h>
#include <my_mem.h>
#include <my_sync.h>
#include <my_time.h>
#include <string.h>
#include <lib.h>
#include <input.h>

void helpCmd(void);
void killCmd(int argc, char *argv[]);
void blockCmd(int argc, char *argv[]);
void unblockCmd(int argc, char *argv[]);
void niceCmd(int argc, char *argv[]);
pid_t loopCmd(int argc, char *argv[], int mode, int new_stdin, int new_stdout);
pid_t printArgsCmd(int argc, char *argv[], int mode, int new_in, int new_out);
pid_t catCmd(int argc, char *argv[], int mode, int new_stdin, int new_stdout);
pid_t filterCmd(int argc, char *argv[], int mode, int new_stdin, int new_stdout);
pid_t wcCmd(int argc, char *argv[], int mode, int new_stdin, int new_stdout);
void broombroom(void);
#endif