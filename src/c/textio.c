#include "header/textio.h"

void puts(char *string) {
    int i, scrollLine = 0;
    int width_cap = 80;
    for (i = 0; i < strlen(string); i++) {
        if (string[i] == '\r' || string[i] == '\n') {
            cursor_x = 0;
            cursor_y += 0x0100;
            if (cursor_y >= 0x1900) {
                scrollLine = div(cursor_y - 0x1800, 0x100);
                interrupt(0x10, 0x0600 + scrollLine, 0x0000, 0x0, SCREEN_WIDTH + SCREEN_HEIGHT);
                cursor_y = 0x1900 - (scrollLine * 0x100);
            }
            interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y);
        } else {
            int AX = 0x0E00 + string[i];
            interrupt(0x10, AX, 0x0007, 0x0, 0x0);
            cursor_x++;
        }
    }
}

void putsColor(char *string, int color) {
    int i, scrollLine = 0;
    int width_cap = 80;
    for (i = 0; i < strlen(string); i++) {
        if (string[i] == '\r' || string[i] == '\n') {
            cursor_x = 0;
            cursor_y += 0x0100;
            if (cursor_y >= 0x1900) {
                scrollLine = div(cursor_y - 0x1800, 0x100);
                interrupt(0x10, 0x0600 + scrollLine, 0x0000, 0x0, SCREEN_WIDTH + SCREEN_HEIGHT);
                cursor_y = 0x1900 - (scrollLine * 0x100);
            }
            interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y);
        } else {
            int AX = 0x0E00 + string[i];
            interrupt(0x10, AX, 0x0000 + color, 0x0, 0x0);
            cursor_x++;
        }
    }
}

void putsNumber(int number) {
    char string[10];
    int i = 0;
    int j = 0;
    while (number != 0) {
        string[i] = mod(number, 10) + '0';
        number = div(number, 10);
        i++;
    }
    string[i] = '\0';
    for (j = i - 1; j >= 0; j--) {
        int AX = 0x0E00 + string[j];
        interrupt(0x10, AX, 0x0007, 0x0, 0x0);
        cursor_x++;
    }
}

void gets(char *string) {
    int AX, scrollLine, num, input, delta;
    int i = 0;
    int j = 0;
    int currentBufLength = 0;
    while (true) {
        input = interrupt(0x16, 0x00, 0x00, 0x00, 0x00);
        num = mod(input, 0x100);
        input = div(input, 0x100);
        if (num == 0) {
            if (input == 0x48 && i > 0) {
                cursor_x -= i;
                interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y + cursor_x);
                i = 0;
            } else if (input == 0x50 && i < currentBufLength) {
                cursor_x += currentBufLength - i;
                interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y + cursor_x);
                i = currentBufLength;
            } else if (input == 0x4B && i > 0) {
                cursor_x--;
                interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y + cursor_x);
                i--;
            } else if (input == 0x4D && i < currentBufLength) {
                cursor_x++;
                interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y + cursor_x);
                i++;
            }
            continue;
        } else if (num == 13) {
            cursor_x = 0;
            cursor_y += 0x0100;
            if (cursor_y >= 0x1900) {
                scrollLine = div(cursor_y - 0x1800, 0x100);
                interrupt(0x10, 0x0600 + scrollLine, 0x0000, 0x0, SCREEN_WIDTH + SCREEN_HEIGHT);
                cursor_y = 0x1900 - (scrollLine * 0x100);
                interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y);
            }
            break;
        } else if (num == 8) {
            if (i > 0) {
                cursor_x--;
                interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y + cursor_x);
                interrupt(0x10, 0x0A00, 0x0000, i + 1, 0x0);
                i--;
                currentBufLength--;
                for (j = i; j < currentBufLength; j++) {
                    string[j] = string[j + 1];
                    interrupt(0x10, 0x0A00 + string[j], 0x0000, 0x1, cursor_y + cursor_x + j - 1);
                }
                interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y + cursor_x);
                string[i + 1] = 0;
            }
            continue;

        } else if (num == 27) {
            clear(string, i + 1);
            currentBufLength -= i;
            cursor_x -= i;
            interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y + cursor_x);
            interrupt(0x10, 0x0A00, 0x0000, i + 1, 0x0);
            i = 0;
            continue;
        }
        delta = i - currentBufLength;
        for (j = i; j < currentBufLength; j++) {
            cursor_x++;
            interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y + cursor_x);
            interrupt(0x10, 0x0A00 + string[j], 0x0000, 0x1, 0x0);
        }
        for (j = currentBufLength; j > i; j--) {
            string[j] = string[j - 1];
            cursor_x--;
        }
        interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y + cursor_x);
        string[i] = num;

        // print character on the screen
        AX = 0x0E00 + num;
        interrupt(0x10, AX, 0x0007, 0x0, 0x0);
        i++;
        cursor_x++;
        currentBufLength++;
    }
    i = currentBufLength;
    string[i] = '\0';
    interrupt(0x10, 0x0200, 0x00, 0x0, cursor_y);
}
