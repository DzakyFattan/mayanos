#include "header/std_lib.h"

// Opsional : Tambahkan implementasi kode C
int div(int a, int b) {
    return a / b;
}

int mod(int a, int n) {
    return (a - n * (a / n));
}

void memcpy(byte *dest, byte *src, unsigned int n) {
    int i;
    for (i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// Mengembalikan panjang suatu null terminated string
unsigned int strlen(char *string) {
    int i;
    for (i = 0; string[i] != '\0'; i++)
        ;
    return i;
}

bool strcmp(char *s1, char *s2) {
    int i;
    for (i = 0; i < strlen(s2); i++) {
        if (s1[i] != s2[i]) {
            return false;
        }
    }
    return true;
}

void strcpy(char *dst, char *src) {
    int i;
    for (i = 0; i < strlen(src); i++) {
        dst[i] = src[i];
    }
}

void clear(byte *ptr, unsigned int n) {
    int i;
    for (i = 0; i < n; i++) {
        ptr[i] = 0;
    }
}
