// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    char buf[128];
    void fillMap();
    makeInterrupt21();
    clearScreen();

    printString("Hello, World! This is MayanOS!! \r\n");
    readString(buf);
    printString(buf);

    while (true)
        ;
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    switch (AX)
    {
    case 0x0:
        printString(BX);
        break;
    case 0x1:
        readString(BX);
        break;
    case 0x2:
        readSector(BX, CX);
        break;
    case 0x3:
        writeSector(BX, CX);
        break;
    case 0x4:
        read(BX, CX);
        break;
    case 0x5:
        write(BX, CX);
        break;
    default:
        printString("Invalid Interrupt");
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

void clearScreen() {
    interrupt(0x10, 0x00, 0, 0, 0);
    interrupt(0x10, 0x03, 0, 0, 0);
    interrupt(0x10, 0x0200, 0x0, 0x0, 0x0);
}

void writeSector(byte *buffer, int sector_number) {
    int sector_write_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector = mod(sector_number, 18) + 1;    // CL

    head = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00;                               // DL

    interrupt(
        0x13,                        // Interrupt number
        0x0300 | sector_write_count, // AX
        buffer,                      // BX
        cylinder | sector,           // CX
        head | drive                 // DX
    );
}

void readSector(byte *buffer, int sector_number) {
    int sector_read_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector = mod(sector_number, 18) + 1;    // CL

    head = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00;                               // DL

    interrupt(
        0x13,                       // Interrupt number
        0x0200 | sector_read_count, // AX
        buffer,                     // BX
        cylinder | sector,          // CX
        head | drive                // DX
    );
}

void fillMap() {
    struct map_filesystem map_fs_buffer;
    int i;

    // Load filesystem map
    readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);

    /*
      Edit filesystem map disini
                               */
    for (i = 0; i <= 15; i++)
    {
        map_fs_buffer.is_filled[i] = true;
    }

    for (i = 256; i <= 511; i++)
    {
        map_fs_buffer.is_filled[i] = true;
    }

    // Update filesystem map
    writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
}

void write(struct file_metadata *metadata, enum fs_retcode *return_code) {
    struct node_filesystem node_fs_buffer;
    struct sector_filesystem sector_fs_buffer;
    struct map_filesystem map_fs_buffer;
    // Tambahkan tipe data yang dibutuhkan

    // Masukkan filesystem dari storage ke memori

    // 1. Cari node dengan nama dan lokasi parent yang sama pada node.
    //    Jika tidak ditemukan kecocokan, lakukan proses ke-2.
    //    Jika ditemukan node yang cocok, tuliskan retcode
    //    FS_W_FILE_ALREADY_EXIST dan keluar.

    // 2. Cari entri kosong pada filesystem node dan simpan indeks.
    //    Jika ada entry kosong, simpan indeks untuk penulisan.
    //    Jika tidak ada entry kosong, tuliskan FS_W_MAXIMUM_NODE_ENTRY
    //    dan keluar.

    // 3. Cek dan pastikan entry node pada indeks P adalah folder.
    //    Jika pada indeks tersebut adalah file atau entri kosong,
    //    Tuliskan retcode FS_W_INVALID_FOLDER dan keluar.

    // 4. Dengan informasi metadata filesize, hitung sektor-sektor
    //    yang masih kosong pada filesystem map. Setiap byte map mewakili
    //    satu sektor sehingga setiap byte mewakili 512 bytes pada storage.
    //    Jika empty space tidak memenuhi, tuliskan retcode
    //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
    //    Jika ukuran filesize melebihi 8192 bytes, tuliskan retcode
    //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
    //    Jika tersedia empty space, lanjutkan langkah ke-5.

    // 5. Cek pada filesystem sector apakah terdapat entry yang masih kosong.
    //    Jika ada entry kosong dan akan menulis file, simpan indeks untuk
    //    penulisan.
    //    Jika tidak ada entry kosong dan akan menulis file, tuliskan
    //    FS_W_MAXIMUM_SECTOR_ENTRY dan keluar.
    //    Selain kondisi diatas, lanjutkan ke proses penulisan.

    // Penulisan
    // 1. Tuliskan metadata nama dan byte P ke node pada memori buffer
    // 2. Jika menulis folder, tuliskan byte S dengan nilai
    //    FS_NODE_S_IDX_FOLDER dan lompat ke langkah ke-8
    // 3. Jika menulis file, tuliskan juga byte S sesuai indeks sector
    // 4. Persiapkan variabel j = 0 untuk iterator entry sector yang kosong
    // 5. Persiapkan variabel buffer untuk entry sector kosong
    // 6. Lakukan iterasi berikut dengan kondisi perulangan (penulisan belum selesai && i = 0..255)
    //    1. Cek apakah map[i] telah terisi atau tidak
    //    2. Jika terisi, lanjutkan ke iterasi selanjutnya / continue
    //    3. Tandai map[i] terisi
    //    4. Ubah byte ke-j buffer entri sector dengan i
    //    5. Tambah nilai j dengan 1
    //    6. Lakukan writeSector() dengan file pointer buffer pada metadata
    //       dan sektor tujuan i
    //    7. Jika ukuran file yang telah tertulis lebih besar atau sama dengan
    //       filesize pada metadata, penulisan selesai

    // 7. Lakukan update dengan memcpy() buffer entri sector dengan
    //    buffer filesystem sector
    // 8. Lakukan penulisan seluruh filesystem (map, node, sector) ke storage
    //    menggunakan writeSector() pada sektor yang sesuai
    // 9. Kembalikan retcode FS_SUCCESS
}

void read(struct file_metadata *metadata, enum fs_retcode *return_code) {
    struct node_filesystem node_fs_buffer;
    struct sector_filesystem sector_fs_buffer;
    // Tambahkan tipe data yang dibutuhkan

    // Masukkan filesystem dari storage ke memori buffer

    // 1. Cari node dengan nama dan lokasi yang sama pada filesystem.
    //    Jika ditemukan node yang cocok, lanjutkan ke langkah ke-2.
    //    Jika tidak ditemukan kecocokan, tuliskan retcode FS_R_NODE_NOT_FOUND
    //    dan keluar.

    // 2. Cek tipe node yang ditemukan
    //    Jika tipe node adalah file, lakukan proses pembacaan.
    //    Jika tipe node adalah folder, tuliskan retcode FS_R_TYPE_IS_FOLDER
    //    dan keluar.

    // Pembacaan
    // 1. memcpy() entry sector sesuai dengan byte S
    // 2. Lakukan iterasi proses berikut, i = 0..15
    // 3. Baca byte entry sector untuk mendapatkan sector number partisi file
    // 4. Jika byte bernilai 0, selesaikan iterasi
    // 5. Jika byte valid, lakukan readSector()
    //    dan masukkan kedalam buffer yang disediakan pada metadata
    // 6. Lompat ke iterasi selanjutnya hingga iterasi selesai
    // 7. Tulis retcode FS_SUCCESS pada akhir proses pembacaan.
}

void shell() {
    char input_buf[64];
    char path_str[128];
    byte current_dir = FS_NODE_P_IDX_ROOT;

    while (true)     {
        printString("OS@IF2230:");
        // printCWD(path_str, current_dir);
        printString("$");
        readString(input_buf);

        // if (strcmp(input_buf, "cd") {
        //       // Utility cd
        // }
        // else 
        //   printString("Unknown command\r\n");
    }
}
