#include <string.h>
#include <my_mem.h>

int8_t strcmp(const char *p1, const char *p2) {
  const unsigned char *s1 = (const unsigned char *)p1;
  const unsigned char *s2 = (const unsigned char *)p2;
  unsigned char c1, c2;
  do {
    c1 = (unsigned char)*s1++;
    c2 = (unsigned char)*s2++;
    if (c1 == '\0')
      return c1 - c2;
  } while (c1 == c2);
  return c1 - c2;
}

uint8_t strlen(const char *str) {
  const char *s;
  for (s = str; *s; ++s)
    ;
  return (s - str);
}

int parser(char *input, char *argv[]) {
  int len = 0, j = 0;
  char *arg = input;
  for(int i = 0; input[i]; i++) {
    if(input[i] != ' ')
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