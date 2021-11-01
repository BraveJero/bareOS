#ifndef __SHELL_CMDS_H__
#define __SHELL_CMDS_H__

#include <utils.h>
#include <my_process.h>
#include <my_mem.h>
#include <my_sync.h>
#include <string.h>
#include <lib.h>
#include <input.h>

void helpCmd(void);
void killCmd(int argc, char *argv[]);
void blockCmd(int argc, char *argv[]);
void unblockCmd(int argc, char *argv[]);
void niceCmd(int argc, char *argv[]);
void loopCmd(int argc, char *argv[], int mode, int new_stdin, int new_stdout);
void catCmd(int argc, char *argv[], int mode, int new_stdin, int new_stdout);
void filterCmd(int argc, char *argv[], int mode, int new_stdin, int new_stdout);
void wcCmd(int argc, char *argv[], int mode, int new_stdin, int new_stdout);

#endif