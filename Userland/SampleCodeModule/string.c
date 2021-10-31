#include <string.h>
#include <my_mem.h>

int8_t strcmp(const char *str1, const char *str2) {
    while(*str1 || *str2)
        if (*(str1++) ^ *(str2++)) return *(str2-1) - *(str1-1);
    return 0;
}

uint8_t strlen(const char *str) {
  const char *s;
  for (s = str; *s; ++s)
    ;
  return (s - str);
}

int parser(char *input, char *argv[]) {
  int len = 0, j = 0, i = 0;
  char *arg = input;
  while(input[i] == ' ') {
    i++; arg++;
  }
  for(; input[i] && j < MAX_ARGS - 1; i++) {
    if (input[i] != ' ')
      len++;
    else {
      argv[j] = arg;
      argv[j][len] = '\0';
      len = 0;
      arg = input + i + 1;
      j++;
    }
  }
  argv[j] = arg;
  argv[j][len] = '\0';
  j++;
  argv[j] = NULL;
  return j;
}