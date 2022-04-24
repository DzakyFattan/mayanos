#include "header/textio.h"

void puts(char *string) {
    interrupt(0x21, 0x0, string, 0, 0);
}

void putsColor(char *string, int color) {
    interrupt(0x21, 0xB, string, color, 0);
}

void putsNumber(int number) {
    interrupt(0x21, 0xD, number, 0, 0);
}

void gets(char *string) {
    interrupt(0x21, 0x1, string, 0, 0);
}
