#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "std_type.h"
#include "filesystem.h"

#define MSG_SECTOR 0x104

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

void exec(struct file_metadata *metadata, int segment);
// execute a program from given metadata

void exit();
// exit the program

// set message
void setMessage(struct message *msg);

void getMessage(struct message *msg);

#endif