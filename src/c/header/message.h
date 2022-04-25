#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "std_type.h"

#define MSG_SECTOR 0x104

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

struct message {
    byte current_directory;
    char arg1[64];
    char arg2[64];
    char arg3[64];
    char arg4[64];
    int next_program_segment;
};

void setMessage(struct message *msg);

void getMessage(struct message *msg);


#endif