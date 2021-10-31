#ifndef __SHELL_CMDS_H__
#define __SHELL_CMDS_H__

#include <utils.h>
#include <my_process.h>
#include <my_mem.h>
#include <my_sync.h>
#include <string.h>
#include <lib.h>

void helpCmd(void);
void killCmd(int argc, char *argv[]);
void blockCmd(int argc, char *argv[]);
void unblockCmd(int argc, char * argv[]);
void loopCmd(int argc, char *argv[]);

#endif