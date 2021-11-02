// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kstring.h>

uint64_t strlen(char *s){
    int i = 0;
    while(s[i++]);
    return i - 1;
}

void strcpy(char *s, char *t){
    while(*t != 0){
        *(s++) = *(t++);
    }
    *s = 0;
}
