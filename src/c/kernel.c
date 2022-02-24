// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    char *string = "Hello, World! This is MayanOS";
    // Set Video Mode
    interrupt(0x10, 0x03, 0, 0, 0);
    printString(string);
    interrupt(0x10, 0x0200, 0x00, 0x0, 0x0002);
    clearScreen();

    while (true)
        ;
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    // Definisi kosong
}

void printString(char *string) {
    int i = 0;
    for (i = 0; i < strlen(string); i++) {
        int AX = 0x0E00 + string[i];
        interrupt(0x10, AX, 0x0006, 0x0, 0x0);
    }
}

void clearScreen() {
    interrupt(0x10, 0x00, 0, 0, 0);
    interrupt(0x10, 0x03, 0, 0, 0);
    interrupt(0x10, 0x0200, 0x00, 0x0, 0x00);
}