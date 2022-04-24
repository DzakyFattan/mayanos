#ifndef _TEXTIO_H
#define _TEXTIO_H

#include "std_type.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

void puts(char *string);
// Menampilkan string ke layar

void putsColor(char *string, int color);
// Menampilkan string dengan warna tertentu

void putsNumber(int number);
// Menampilkan angka ke layar

void gets(char *string);
// Meminta input dari user

#endif