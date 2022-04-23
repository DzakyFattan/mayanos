#ifndef _FILEIO_H
#define _FILEIO_H

#include "std_type.h"
#include "filesystem.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

void write(struct file_metadata *metadata, enum fs_retcode *return_code);
// Write the file to the disk.
void read(struct file_metadata *metadata, enum fs_retcode *return_code);
// Read the file from the disk.

#endif