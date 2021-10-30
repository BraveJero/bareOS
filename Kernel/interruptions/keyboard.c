#include <keyboard.h>
#include <lib.h>
#include <naiveConsole.h>

#define LEFT_SHIFT 0x2A
#define LEFT_SHIFT_FLAG 0b00000001
#define RIGHT_SHIFT 0x36
#define RIGHT_SHIFT_FLAG 0b00000010
#define LEFT_ALT 0x38

#define BUFFER_SIZE 256

uint8_t flags = 0;

typedef uint8_t BufferPtr;

static int8_t buffer[BUFFER_SIZE] = {0};
static BufferPtr w_pointer = 0, r_pointer = 0;
static uint16_t write, lock;
static uint16_t readers = 0;

int initKeyboard(void) {
  write = sem_open(KEYBOARD_WRITE_SEM, 0);
  if (write == -1)
    return -1;
  lock = sem_open(KEYBOARD_BLOCK_SEM, 1);
  if (lock == -1)
    return -1;
  return 0;
}

// https://stanislavs.org/helppc/make_codes.html
int8_t lowerScancodeToAscii[128] = {

    0,   27,   '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-',
    '=', '\b', -1,   'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']',  '\n', 0,   'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`',  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
    '.', '/',  0,    '*', 0,    ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   0,   0,   0,   '-', 0,   0,   0,
    '+', 0,    0,    0,   0,    0,   0,   0,   0,   0,   0,   0

};

int8_t upperScancodeToAscii[128] = {

    0,   27,   '!',  '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',
    '+', '\b', -1,   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    '{', '}',  '\n', 0,   'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
    ':', '"',  '~',  0,   '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<',
    '>', '?',  0,    '*', 0,   ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,   0,   0,   0,   0,   '-', 0,   0,   0,
    '+', 0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0

};

static void appendBuffer(int8_t c) {
  if (sem_wait(lock) < 0)
    return;

  buffer[w_pointer++] = c;

  while (readers){
    readers--;
    if (sem_post(write) < 0){
      return;
    }
  }

  if (sem_post(lock) < 0)
    return;
  return;
}

void keyboard_handler() {
  uint8_t scancode = getKey();
  // ncPrintHex(scancode);
  if (scancode < 0x80) { // Make/Pressed
    if (scancode == LEFT_SHIFT) {
      flags |= LEFT_SHIFT_FLAG;
    } else if (scancode == RIGHT_SHIFT) {
      flags |= RIGHT_SHIFT_FLAG;
    } else {
      if ((flags & LEFT_SHIFT_FLAG) || (flags & RIGHT_SHIFT_FLAG))
        // ncPrintChar(upperScancodeToAscii[scancode]);
        appendBuffer(upperScancodeToAscii[scancode]);
      else
        // ncPrintChar(lowerScancodeToAscii[scancode]);
        appendBuffer(lowerScancodeToAscii[scancode]);
    }
  } else { // Break/Released
    scancode -= 0x80;
    if (scancode == LEFT_SHIFT)
      flags &= ~LEFT_SHIFT_FLAG;
    else if (scancode == RIGHT_SHIFT)
      flags &= ~RIGHT_SHIFT_FLAG;
  }
}

// int64_t getChar(void) {
//   if (head != tail) {
//     char ans = buffer[tail++];
//     if (tail == BUFFER_SIZE)
//       tail = 0;
//       return ans == 0 ? -1 : ans;
//   }
//   return -1;
// }

long stdRead(char *buf, size_t count) {
  if (count > BUFFER_SIZE) {
    return 0;
  }

  long i = 0;

  if (sem_wait(lock) < 0)
    return -1;

  while (r_pointer == w_pointer) {
    readers++;
    if (sem_post(lock) < 0)
      return -1;
    if (sem_wait(write) < 0)
      return -1;
    if (sem_wait(lock) < 0)
      return -1;
  }

  while (i < count && r_pointer != w_pointer)
    buf[i++] = buffer[r_pointer++];

  if (sem_post(lock) < 0)
    return -1;

  if (i < count)
    buf[i] = '\0';

  return i;
}