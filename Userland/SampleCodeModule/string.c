#include <my_mem.h>
#include <string.h>
#include <lib.h>

int8_t strcmp(const char *str1, const char *str2) {
  while (*str1 || *str2)
    if (*(str1++) ^ *(str2++))
      return *(str2 - 1) - *(str1 - 1);
  return 0;
}

uint8_t strlen(const char *str) {
  const char *s;
  for (s = str; *s; ++s)
    ;
  return (s - str);
}

char *strchr(const char *s, char c) {
  while(*s) {
    if(*s == c)
      return s;
    s++;
  }
  return NULL;
}

int parser(char *input, char *argv[]) {
  int len = 0, j = 0, i = 0;
  char *arg = input;
  while (input[i] == ' ') {
    i++;
    arg++;
  }
  for (; input[i] && j < MAX_ARGS - 1; i++) {
    if (input[i] != ' ')
      len++;
    else {
      while (input[i] == ' ')
        i++;
      i--;

      argv[j] = arg;
      argv[j][len] = '\0';
      len = 0;
      arg = input + i + 1;
      j++;
    }
  }
  if (input[i - 1] != '\0' &&  input[i - 1] != ' ') {
    argv[j] = arg;
    argv[j][len] = '\0';
    j++;
  }
  argv[j] = NULL;
  return j;
}

int findCmd(char *in, char *cmds[], int dim) {
  for(int i = 0; i < dim; i++) {
    if(strcmp(in, cmds[i]) == 0)
      return i;
  }
  return -1;
}