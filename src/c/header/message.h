#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "std_type.h"
#include "fileio.h"
#include "utils.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

struct message {
    byte current_directory;
    char next_command[9];
    int next_program_segment;
    char arg1[64];
    char arg2[64];
    char arg3[64];
    char arg4[64];
    char other[244];
};

void setMessage(struct message *msg, int segment);

void getMessage(struct message *msg, int segment);

#endif