#include "header/string.h"

unsigned int strlen(char *string) {
    unsigned int length = interrupt(0x21, 0x6, string, 0, 0);
    return length;
}

bool strcmp(char *s1, char *s2) {
    bool result = interrupt(0x21, 0x7, s1, s2, 0);
    return result;
}

void strcpy(char *dst, char *src) {
    interrupt(0x21, 0x8, dst, src, 0);
}

void strclr(char *string) {
    interrupt(0x21, 0x9, string, 0, 0);
}
