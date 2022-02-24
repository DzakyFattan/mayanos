// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    // char *string = "This is MayanOS "; // Deklarasikan variabel pada awal scope
    // int i = 0;
    // int strLen;
    // for (strLen = 0; string[strLen] != '\0'; ++strLen) // Hitung panjang string
    //     ;
    // for (i = 0; i < strLen; i++) {
    //     byte warna = 0xD;
    //     putInMemory(0xB000, 0x8000 + 2 * i, string[i]);
    //     putInMemory(0xB000, 0x8001 + 2 * i, warna);
    // }

    interrupt(0x10, 0x0E54, 0x0003, 0x0, 0x0);
    interrupt(0x10, 0x0E45, 0x0003, 0x0, 0x0);
    interrupt(0x10, 0x0E53, 0x0003, 0x0, 0x0);
    interrupt(0x10, 0x0E54, 0x0003, 0x0, 0x0);

    while (true)
        ;
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    // Definisi kosong
}