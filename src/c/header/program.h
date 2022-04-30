#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "std_type.h"
#include "filesystem.h"
#include "message.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

void exec(struct file_metadata *metadata, int segment);
// execute a program from given metadata

void exit();
// exit the program

#endif