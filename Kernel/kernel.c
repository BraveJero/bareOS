// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <idtLoader.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <string.h>
#include <userland.h>
#include <video.h>
#include <scheduler.h>
#include <process.h>
#include <time.h>
#include <mmgr.h>
#include <interrupts.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

void clearBSS(void *bssAddress, uint64_t bssSize) {
  memset(bssAddress, 0, bssSize);
}

void *getStackBase() {
  return (void *)((uint64_t)&endOfKernel +
                  PageSize * 8       // The size of the stack itself, 32KiB
                  - sizeof(uint64_t) // Begin at the top of the stack
  );
}


void *initializeKernelBinary() {
  
  void *moduleAddresses[] = {sampleCodeModuleAddress, sampleDataModuleAddress};

  loadModules(&endOfKernelBinary, moduleAddresses);

  clearBSS(&bss, &endOfKernel - &bss);

  return getStackBase();
}

int main() {
  init_screen();
  prompt_info prompt = {.x = 0,
                        .y = 0,
                        .baseX = 0,
                        .baseY = 0,
                        .windowWidth = getScreenWidth(),
                        .windowHeight = getScreenHeight()};
  declarePrompt(&prompt);

  initMgr();
  initKeyboard();
  initScheduler();

  char *argv[2] = {"shell", NULL};
  pid_t pid = createProcess((uint64_t)sampleCodeModuleAddress, 10, 1, argv, MASK_FOREGROUND);
  exec(pid);

  load_idt();
  while(1) { _hlt(); }


  return 0;
}
