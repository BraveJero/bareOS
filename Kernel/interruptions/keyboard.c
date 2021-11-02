// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboard.h>

uint8_t flags = 0;

typedef uint8_t BufferPtr;

static int8_t buffer[BUFFER_SIZE] = {0};
static BufferPtr w_pointer = 0, r_pointer = 0;
static uint16_t write, lock, reading;

int initKeyboard(void) {
  write = sem_open(KEYBOARD_WRITE_SEM, 0);
  if (write == -1)
    return -1;
  lock = sem_open(KEYBOARD_BLOCK_SEM, 1);
  if (lock == -1) {
    sem_close(write);
    return -1;
  }
  reading = sem_open(KEYBOARD_READING_SEM, 1);
  if (reading == -1) {
    sem_close(write);
    sem_close(lock);
    return -1;
  }
  return 0;
}

// https://stanislavs.org/helppc/make_codes.html
int8_t lowerScancodeToAscii[128] = {

    0,   27,   '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-',
    '=', '\b', EOF,  'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']',  '\n', 0,   'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`',  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
    '.', '/',  0,    '*', 0,    ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   0,   0,   0,   '-', 0,   0,   0,
    '+', 0,    0,    0,   0,    0,   0,   0,   0,   0,   0,   0

};

int8_t upperScancodeToAscii[128] = {

    0,   27,   '!',  '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',
    '+', '\b', EOF,  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
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

  if (sem_post(write) < 0) {
    return;
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

  if (sem_wait(reading) < 0)
    return -1;

  while (i < count) {
    if (sem_wait(write) < 0)
      return -1;

    if (buffer[r_pointer] == EOF) {
      r_pointer++;
      break;
    }

    if (buffer[r_pointer] == '\b') {
      if (i > 0) {
        i--;
        ncPrintChar(buffer[r_pointer]);
      }
    } else {
      buf[i++] = buffer[r_pointer];
      ncPrintChar(buf[i - 1]);
      if (buf[i - 1] == '\n') {
        r_pointer++;
        break;
      }
    }
    r_pointer++;
  }

  if (sem_post(reading) < 0)
    return -1;

  return i;
}
