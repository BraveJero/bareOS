#include <input.h>

void cat(void) {
  char buffer[MAX_BUF_SIZE] = {0};

  int32_t counter = 0;
  int64_t c = 1;
  while ((c = get_char()) != -1) {
    if (c == '\n') {
      buffer[counter++] = c;
      print_to_screen((char *)&c, 1);
      for (int32_t i = 0; i < counter; i++) {
        put_char(STDOUT_FILENO, buffer[i]);
      }
      counter = 0;
    } else {
      if (counter < MAX_BUF_SIZE - 1) {
        if (c == '\b') { // Backspace
          if (counter == 0)
            continue;
          counter--;
        } else {
          buffer[counter++] = c;
        }
      } else {
        if (c == '\b')
          counter--;
        else
          break;
      }
      print_to_screen((char *)&c, 1);
    }
  }
  if (counter != 0) {
    c = '\n';
    print_to_screen((char *)&c, 1);
    for (int32_t i = 0; i < counter; i++) {
      put_char(STDOUT_FILENO, buffer[i]);
    }
  }
  exit();
}
