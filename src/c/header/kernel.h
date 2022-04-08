// Kernel header

#include "filesystem.h"
#include "std_lib.h"
#include "std_type.h"

// variabel untuk tracking posisi cursor
extern int cursor_x = 0x0;
extern int cursor_y = 0x0;

// Fungsi bawaan
extern void putInMemory(int segment, int address, byte b);
extern int interrupt(int int_number, int AX, int BX, int CX, int DX);
extern void makeInterrupt21();
extern void launchProgram(int segment);

void executeProgram(struct file_metadata *metadata, int segment);

void handleInterrupt21(int AX, int BX, int CX, int DX);
void fillKernelMap();
void fillMap();
void printNumber(int number);

// read-write string
void printString(char *string);
void readString(char *string);

// clear screen
void clearScreen();

// read-write tp memory
void writeSector(byte *buffer, int sector_number);
void readSector(byte *buffer, int sector_number);

void write(struct file_metadata *metadata, enum fs_retcode *return_code);
void read(struct file_metadata *metadata, enum fs_retcode *return_code);

// commands
void shell();
void printCWD(char *path_str, byte current_dir);

void cd(char *path_str, byte *current_dir);
void ls(byte current_dir);
void copy(char *input_buf, byte current_dir);
void mkdir(char *input_buf, byte current_dir);
void cat(char *input_buf, byte current_dir);
void move(char *input_buf, byte current_dir);

void scrollController(int lines);
