// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboard.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <time.h>

void (*handlers[15])(void) = {timer_handler,    // int 20h
                              keyboard_handler, // int 21h
                              NULL};

void irqDispatcher(uint64_t irq) {
  void (*interruptionHandler)(void) = handlers[irq];
  if (interruptionHandler != NULL)
    (*interruptionHandler)();
  return;
}