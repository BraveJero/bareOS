// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <input.h>

#define IS_VOWEL(c)                                                            \
  (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' || c == 'a' ||     \
   c == 'e' || c == 'i' || c == 'o' || c == 'u')

void cat(void) {
  char buffer[MAX_BUF_SIZE] = {0};
  int read = 0;
  do {
    if ((read = get_s(buffer, MAX_BUF_SIZE)) < 0) {
      put_s(STDERR_FILENO, "\nError while reading.\n");
      exit();
    }
    print(STDOUT_FILENO, buffer, read);
  } while (read != 0 && buffer[read - 1] == '\n');
  exit();
}

void filter(void) {
  char buf[MAX_BUF_SIZE] = {0};
  int read = 0;
  do {
    if ((read = get_s(buf, MAX_BUF_SIZE)) < 0) {
      put_s(STDERR_FILENO, "\nError while reading.\n");
      exit();
    }
    for (int i = 0; i < read && buf[i]; i++) {
      if (!IS_VOWEL(buf[i]))
        put_char(STDOUT_FILENO, buf[i]);
    }
  } while (read != 0 && buf[read - 1] == '\n');
  exit();
}

void wc(void) {
  char buf[MAX_BUF_SIZE] = {0};
  int lineCounter = 0, byteCounter = 0, read;
  do {
    if ((read = get_s(buf, MAX_BUF_SIZE)) < 0) {
      put_s(STDERR_FILENO, "\nError while reading.\n");
      exit();
    }
    if (read > 0)
      lineCounter++;
    byteCounter += read;
  } while (read != 0 && buf[read - 1] == '\n');
  print_f(STDOUT_FILENO, "Read %d bytes in %d lines\n", byteCounter,
          lineCounter);
  exit();
}
