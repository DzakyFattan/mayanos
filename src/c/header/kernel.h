// Kernel header

#include "color.h"
#include "filesystem.h"
#include "std_lib.h"
#include "std_type.h"
#include "message.h"

// screen size constant
#define SCREEN_HEIGHT 0x1900
#define SCREEN_WIDTH 0x50

// Fungsi bawaan
extern void putInMemory(int segment, int address, byte b);
extern int interrupt(int int_number, int AX, int BX, int CX, int DX);
extern void makeInterrupt21();
extern void launchProgram(int segment);

int handleInterrupt21(int AX, int BX, int CX, int DX);
void fillMap();
void executeProgram(struct file_metadata *metadata, int segment);
void exit();

// read-write string
void printString(char *string);
void printColor(char *string, int color);
void printNumber(int number);
void readString(char *string);

// clear screen
void clearScreen();

// read-write tp memory
void writeSector(byte *buffer, int sector_number);
void readSector(byte *buffer, int sector_number);

void write(struct file_metadata *metadata, enum fs_retcode *return_code);
void read(struct file_metadata *metadata, enum fs_retcode *return_code);

void printCWD(byte current_dir);

void splash();