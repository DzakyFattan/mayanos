// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    char buf[128];
    clearScreen();
    makeInterrupt21();
    printString("Hello, World! This is MayanOS!! \r\n");
    readString(buf);
    printString(buf);

    while (true);
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    switch (AX) {
    case 0x0:
        printString(BX);
        break;
    case 0x1:
        readString(BX);
        break;
    default:
        printString("Invalid interrupt");
    }
}

void printString(char *string) {
    int i = 0;
    for (i = 0; i < strlen(string); i++)
    {
        int AX = 0x0E00 + string[i];
        interrupt(0x10, AX, 0x0000, 0x0, 0x0);
    }
    interrupt(0x10, 0x0200, 0x00, 0x0, 0x0200);
}

void clearScreen() {
    interrupt(0x10, 0x00, 0, 0, 0);
    interrupt(0x10, 0x03, 0, 0, 0);
    interrupt(0x10, 0x0200, 0x0, 0x0, 0x0);
}

void readString(char *string) {
    int AX, num, i = 0;

    while (true)
    {
        num = interrupt(0x16, 0x00, 0x00, 0x00, 0x00);
        if (num == '\r')
        {
            break;
        }
        string[i] = num;

        // print character on the screen
        AX = 0x0E00 + num;
        interrupt(0x10, AX, 0x0000, 0x0, 0x0);
        i++;
    }
    interrupt(0x10, 0x0200, 0x00, 0x0, 0x0300);
}
