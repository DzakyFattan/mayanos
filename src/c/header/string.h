#ifndef _STRING_H
#define _STRING_H

#include "std_type.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

unsigned int strlen(char *string);
// Mengembalikan panjang suatu null terminated string

bool strcmp(char *s1, char *s2);
// Mengembalikan true jika string sama

void strcpy(char *dst, char *src);
// Melakukan penyalinan null terminated string

void strclr(char *string);
// Melakukan penghapusan string

#endif