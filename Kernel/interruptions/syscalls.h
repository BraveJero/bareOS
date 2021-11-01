#ifndef __SYSCALLS_H__
#define __SYSCALLS_H__

#include <interrupts.h>
#include <keyboard.h>
#include <lib.h>
#include <naiveConsole.h>
#include <pipe.h>
#include <process.h>
#include <scheduler.h>
#include <stddef.h>
#include <stdint.h>
#include <sync.h>
#include <video.h>
#include <mmgr.h>

uint64_t sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                           uint64_t rcx, uint64_t r8, uint64_t rax);

#endif