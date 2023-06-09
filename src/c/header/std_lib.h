#ifndef _STD_LIB_H
#define _STD_LIB_H

#include "std_type.h"

// Operator matematika umum
// Implementasikan
// Tips : Gunakan C integer division
int div(int a, int b);

int mod(int a, int n);

// Operasi standar bahasa C
// Implementasikan
void memcpy(byte *dest, byte *src, unsigned int n);
// Mengcopy n bytes yang ditunjuk src ke dest

unsigned int strlen(char *string);
// Mengembalikan panjang suatu null terminated string

bool strcmp(char *s1, char *s2);
// Mengembalikan true jika string sama

void strcpy(char *dst, char *src);
// Melakukan penyalinan null terminated string

void strclr(char *string);
// Melakukan penghapusan string

void clear(byte *ptr, unsigned int n);
// Mengosongkan byte array yang memiliki panjang n

#endif