// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    char *string = "Hello, World! This is MayanOS";
    // Set Video Mode
    interrupt(0x10, 0x0012, 0, 0, 0);
    printString(string);

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
