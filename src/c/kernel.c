// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    char buf[128];
    enum fs_retcode return_code;
    struct file_metadata metadata_file;
    struct file_metadata metadata_read;
    struct file_metadata metadata_folder;
    byte metadata_buf[1024];
    byte metadata_read_buf[8192];
    int i;

    metadata_file.node_name = "file";
    metadata_file.parent_index = 0xFF;
    metadata_file.filesize = 1024;
    
    metadata_folder.node_name = "folderrr";
    metadata_folder.parent_index = 0xFF;
    metadata_folder.filesize = 0;

    fillMap();
    makeInterrupt21();
    clearScreen();
    printString("\nHello, World! This is MayanOS!!\r\n");

    // DEBUG
    metadata_buf[0] = 'K';
    metadata_buf[1] = 'E';
    metadata_buf[2] = 'Q';
    metadata_buf[3] = 'I';
    metadata_buf[4] = 'N';
    metadata_buf[5] = 'G';
    metadata_buf[6] = 'W';
    metadata_buf[7] = 'A';
    metadata_buf[8] = 'N';
    metadata_buf[9] = 'G';
    metadata_buf[10] = 'Y';
    metadata_buf[11] = 'Y';
    metadata_buf[12] = 'Y';
    metadata_buf[13] = 'Y';
    metadata_buf[14] = 'Y';
    metadata_buf[15] = '\0';
    for (i = 16; i < 300; i++) {
        metadata_buf[i] = 'A';
    } 
    for (i = 400; i < 600; i++) {
        metadata_buf[i] = 'B';
    }
    metadata_file.buffer = metadata_buf;
    write(&metadata_file, &return_code);
    write(&metadata_folder, &return_code);

    metadata_read.node_name = "file";
    metadata_read.parent_index = 0xFF;
    metadata_read.buffer = metadata_read_buf;
    read(&metadata_read, &return_code);
    printString("From file: ");
    printString(metadata_read.buffer);
    printString("\n");
    // END DEBUG

    printString(":::=======  :::====  ::: === :::====  :::= === :::====  :::=== \n");
    printString("::: === === :::  === ::: === :::  === :::===== :::  === :::    \n");
    printString("=== === === ========  =====  ======== ======== ===  ===  ===== \n");
    printString("===     === ===  ===   ===   ===  === === ==== ===  ===     ===\n");
    printString("===     === ===  ===   ===   ===  === ===  ===  ======  ====== \n");

    printString("\nMaya siap membantu Trainer-chan, You Copy?! ( ^ w ^)7\r\n");
    // writeSector("gura sayang", 0x10);
    shell();
    printString("Number test: \n");
    printString("\n");

    shell();
    while (true)
        ;
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    switch (AX) {
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
    int i, scrollLine = 0;
    for (i = 0; i < strlen(string); i++) {
        if (string[i] == '\r' || string[i] == '\n') {
            cursor_x = 0;
            cursor_y += 0x0100;
            if (cursor_y >= 0x1900) {
                scrollLine = div(cursor_y - 0x1800, 0x100);
                scrollController(scrollLine);
                cursor_y = 0x1900 - (scrollLine * 0x100);
            }
            interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y);
        } else {
            int AX = 0x0E00 + string[i];
            interrupt(0x10, AX, 0x0000, 0x0, 0x0);
            cursor_x++;
        }
    }
}

void printNumber(int number) {
    int arr[10];
    int j, r, i = 0;

    if (number == 0) {
        int AX = 0x0E00 + 48;
        interrupt(0x10, AX, 0x0000, 0x0, 0x0);
        cursor_x++;
    }

    while (number != 0) {
        r = mod(number, 10);

        arr[i] = r;
        i++;
        number = number / 10;
    }

    for (j = i - 1; j > -1; j--) {
        int AX = 0x0E00 + arr[j] + 48;
        interrupt(0x10, AX, 0x0000, 0x0, 0x0);
        cursor_x++;
    }
}

void readString(char *string) {
    int AX, scrollLine, num, i = 0;
    int j = 0;
    while (true) {
        num = interrupt(0x16, 0x00, 0x00, 0x00, 0x00);
        if (num == 13) {
            cursor_x = 0;
            cursor_y += 0x0100;
            if (cursor_y >= 0x1900) {
                scrollLine = div(cursor_y - 0x1800, 0x100);
                scrollController(scrollLine);
                cursor_y = 0x1900 - (scrollLine * 0x100);
                interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y);
            }
            break;
        } else if (num == 8) {
            if (i > 0) {
                cursor_x--;
                interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y + cursor_x);
                interrupt(0x10, 0x0A00, 0x0000, i + 1, 0x0);
                string[i] = 0;
                i--;
            }
            continue;

        } else if (num == 27) {
            clear(string, i+1);
            while (i != 0) {
                cursor_x--;
                interrupt(0x10, 0x0200, 0x0, 0x0, cursor_y + cursor_x);
                interrupt(0x10, 0x0A00, 0x0000, i + 1, 0x0);
                i--;
            }
            continue;
        }
        string[i] = num;

        // print character on the screen
        AX = 0x0E00 + num;
        interrupt(0x10, AX, 0x0000, 0x0, 0x0);
        i++;
        cursor_x++;
    }
    interrupt(0x10, 0x0200, 0x00, 0x0, cursor_y);
}

void clearScreen() {
    interrupt(0x10, 0x00, 0, 0, 0);
    interrupt(0x10, 0x03, 0, 0, 0);
    interrupt(0x10, 0x0200, 0x0, 0x0, 0x0);
    cursor_x = 0x0;
    cursor_y = 0x0;
    interrupt(0x10, 0x0700, 0x0700, 0x0, 0x1950);
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
        0x0301 | sector_write_count, // AX
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
    for (i = 0; i <= 15; i++) {
        map_fs_buffer.is_filled[i] = true;
    }

    for (i = 256; i <= 511; i++) {
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
    char *filename = metadata->node_name;
    byte parent = metadata->parent_index;
    byte node_index;
    unsigned int i, j, current_writing_byte;
    bool found = false, finished = false;
    byte parent_node_index;
    byte sector_needed, sector_available, empty_entry_index;
    struct sector_entry sector_entry_buf;

    // Masukkan filesystem dari storage ke memori
    // map
    readSector(map_fs_buffer.is_filled, FS_MAP_SECTOR_NUMBER);
    // node
    readSector(node_fs_buffer.nodes, FS_NODE_SECTOR_NUMBER);
    readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 0x1);
    // sector
    readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);

    // 1. Cari node dengan nama dan lokasi parent yang sama pada node.
    //    Jika tidak ditemukan kecocokan, lakukan proses ke-2.
    //    Jika ditemukan node yang cocok, tuliskan retcode
    //    FS_W_FILE_ALREADY_EXIST dan keluar.
    for (i = 0; i < 64 && !found; i++) {
        if (strcmp(node_fs_buffer.nodes[i].name, filename) && node_fs_buffer.nodes[i].parent_node_index == parent) {
            *return_code = FS_W_FILE_ALREADY_EXIST;
            return;
        }
    }

    // 2. Cari entri kosong pada filesystem node dan simpan indeks.
    //    Jika ada entry kosong, simpan indeks untuk penulisan.
    //    Jika tidak ada entry kosong, tuliskan FS_W_MAXIMUM_NODE_ENTRY
    //    dan keluar.
    found = false;
    for (i = 0; i < 64; i++) {
        if (strlen(node_fs_buffer.nodes[i].name) == 0) {
            node_index = i;
            found = true;
            break;
        }
    }
    if (!found) {
        *return_code = FS_W_MAXIMUM_NODE_ENTRY;
        return;
    }


    // 3. Cek dan pastikan entry node pada indeks P adalah folder.
    //    Jika pada indeks tersebut adalah file atau entri kosong,
    //    Tuliskan retcode FS_W_INVALID_FOLDER dan keluar.
    parent_node_index = metadata->parent_index;

    // Jika file/folder tidak terletak di root
    if (parent_node_index != 0xFF) {
        if (node_fs_buffer.nodes[parent_node_index].sector_entry_index != 0xFF) {
            *return_code = FS_W_INVALID_FOLDER;
            return;
        }
    }

    // 4. Dengan informasi metadata filesize, hitung sektor-sektor
    //    yang masih kosong pada filesystem map. Setiap byte map mewakili
    //    satu sektor sehingga setiap byte mewakili 512 bytes pada storage.
    //    Jika empty space tidak memenuhi, tuliskan retcode
    //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
    //    Jika ukuran filesize melebihi 8192 bytes, tuliskan retcode
    //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
    //    Jika tersedia empty space, lanjutkan langkah ke-5.
    if (metadata->filesize == 0) {
        sector_needed = 0;
    } else {
        // ceil division
        sector_needed = (metadata->filesize + 512 - 1) / 512;
    }

    sector_available = 0;
    for (i = 0; i < 32; i++) {
        if (!map_fs_buffer.is_filled[i]) {
            sector_available++;
        }
    }
    if ((sector_available < sector_needed) || (metadata->filesize > 8192)) {
        *return_code = FS_W_NOT_ENOUGH_STORAGE;
        return;
    }

    // 5. Cek pada filesystem sector apakah terdapat entry yang masih kosong.
    //    Jika ada entry kosong dan akan menulis file, simpan indeks untuk
    //    penulisan.
    //    Jika tidak ada entry kosong dan akan menulis file, tuliskan
    //    FS_W_MAXIMUM_SECTOR_ENTRY dan keluar.
    //    Selain kondisi diatas, lanjutkan ke proses penulisan.
    found = false;
    for (i = 0; i < 32; i++) {
        if (sector_fs_buffer.sector_list[i].sector_numbers[0] == 0) {
            empty_entry_index = i;
            found = true;
            break;
        }
    }
    if (!found && (metadata->filesize > 0)) {
        *return_code = FS_W_MAXIMUM_SECTOR_ENTRY;
        return;
    }

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

    strcpy(node_fs_buffer.nodes[node_index].name, metadata->node_name);
    node_fs_buffer.nodes[node_index].parent_node_index = metadata->parent_index;

    if (metadata->filesize == 0) {
        node_fs_buffer.nodes[node_index].sector_entry_index = FS_NODE_S_IDX_FOLDER;
    } else {
        node_fs_buffer.nodes[node_index].sector_entry_index = empty_entry_index; // 3
        j = 0;                                                                   // 4
        // 5 ada di awal
        // 6
        finished = false;
        i = 0;
        current_writing_byte = 0;
        while (!finished && i <= 255) {
            if (!map_fs_buffer.is_filled[i]) {
                map_fs_buffer.is_filled[i] = true;

                sector_entry_buf.sector_numbers[j] = i;
                j++;
                
                writeSector(&metadata->buffer[current_writing_byte], i);

                current_writing_byte += 512;
                if (metadata->filesize <= current_writing_byte) {
                    finished = true;
                }
            }
            i++;
        }
        // 7
        memcpy(sector_fs_buffer.sector_list[empty_entry_index].sector_numbers, sector_entry_buf.sector_numbers, 16);
    }
    // 8
    writeSector(map_fs_buffer.is_filled, 0x100);
    writeSector(&node_fs_buffer.nodes[0], 0x101);
    writeSector(&node_fs_buffer.nodes[32], 0x102);
    writeSector(sector_fs_buffer.sector_list, 0x103);

    *return_code = FS_SUCCESS;
}

void read(struct file_metadata *metadata, enum fs_retcode *return_code) {
    struct node_filesystem node_fs_buffer;
    struct sector_filesystem sector_fs_buffer;
    char *filename = metadata->node_name;
    byte *test = 0x61;
    byte parent = metadata->parent_index;
    byte node_index;
    byte sector_entry_index;
    byte sector_nums_buffer[16];
    byte i;
    bool found = false;

    // Masukkan filesystem dari storage ke memori buffer
    // node
    readSector(&node_fs_buffer.nodes[0], FS_NODE_SECTOR_NUMBER);
    readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 0x1);

    // sector
    readSector(&sector_fs_buffer.sector_list[0], FS_SECTOR_SECTOR_NUMBER);

    // 1. Cari node dengan nama dan lokasi yang sama pada filesystem.
    //    Jika ditemukan node yang cocok, lanjutkan ke langkah ke-2.
    //    Jika tidak ditemukan kecocokan, tuliskan retcode FS_R_NODE_NOT_FOUND
    //    dan keluar.
    for (i = 0; i < 64 && !found; i++) {
        // printString(node_fs_buffer.nodes[i].name);
        // printString("\n");
        if (strcmp(node_fs_buffer.nodes[i].name, filename) && node_fs_buffer.nodes[i].parent_node_index == parent) {
            node_index = i;
            found = true;
            break;
        }
    }

    if (!found) {
        *return_code = FS_R_NODE_NOT_FOUND;
        return;
    }

    // 2. Cek tipe node yang ditemukan
    //    Jika tipe node adalah file, lakukan proses pembacaan.
    //    Jika tipe node adalah folder, tuliskan retcode FS_R_TYPE_IS_FOLDER
    //    dan keluar.
    sector_entry_index = node_fs_buffer.nodes[node_index].sector_entry_index;

    if (sector_entry_index == FS_NODE_S_IDX_FOLDER) {
        *return_code = FS_R_TYPE_IS_FOLDER;
        return;
    }

    // Pembacaan
    // 1. memcpy() entry sector sesuai dengan byte S
    memcpy(sector_nums_buffer, sector_fs_buffer.sector_list[sector_entry_index].sector_numbers, 16);
    
    // 2. Lakukan iterasi proses berikut, i = 0..15
    // 3. Baca byte entry sector untuk mendapatkan sector number partisi file
    // 4. Jika byte bernilai 0, selesaikan iterasi
    // 5. Jika byte valid, lakukan readSector()
    //    dan masukkan kedalam buffer yang disediakan pada metadata
    // 6. Lompat ke iterasi selanjutnya hingga iterasi selesai
    metadata->filesize = 0;
    for (i = 0; i < 16; i++) {
        if (sector_nums_buffer[i] == 0) {
            break;
        }
        readSector(&(metadata->buffer[i*512]), sector_nums_buffer[i]);
        metadata->filesize += 512;
    }

    // memcpy(*metadata->buffer, test, 1);
    // 7. Tulis retcode FS_SUCCESS pada akhir proses pembacaan.

    *return_code = FS_SUCCESS;
}

void shell() {
    char input_buf[64];
    char path_str[128];

    int i = 0;
    int scrollLine;

    byte current_dir = FS_NODE_P_IDX_ROOT;

    while (true) {
        printString("OS@IF2230:");
        cursor_x = strlen("OS@IF2230:");
        // printCWD(path_str, current_dir);
        printString("$ ");

        readString(input_buf);

        if (strcmp(input_buf, "clear")) {
            clearScreen();
        } else if (strcmp(input_buf, "scroll")) {
            scrollController(1);
        } else if (strcmp(input_buf, "Aku sayang Maya-chin")) {
            printString("Maya juga sayang Trainer-chan ^///^\r\n");
        } else if (strcmp(input_buf, "exit")) {
            break;
        } else if (strcmp(input_buf, "ls")) {
            printString("ls\n");
        } else if (strcmp(input_buf, "cd")) {
            printString("cd\n");
        } else if (strcmp(input_buf, "cp")) {
            copy(input_buf, current_dir);
        }else if (strcmp(input_buf, "mkdir")) {
            printString("mkdir\n");
        } else if (strcmp(input_buf, "rm")) {
            printString("rrmad\n");
        } else if (strcmp(input_buf, "cat")) {
            printString("cat\n");
        } else if (strcmp(input_buf, "write")) {
            printString("write\n");
        } else if (strcmp(input_buf, "read")) {
            printString("read\n");
        } else if (strcmp(input_buf, "curloc")) {
            printNumber(cursor_x);
            printString(" ");
            printNumber(cursor_y);
            printString("\n");
        } else {
            printString("Maya ngga ngerti perintah Trainer-chan (# -_-)\r\n");
        }
        strclr(input_buf);
        strclr(path_str);
    }
}

void scrollController(int lines) {
    int i;
    if (lines < 256) {
        for (i = 0; i < lines; i++) {
            interrupt(0x10, 0x0600 + lines, 0x0700, 0x0, 0x1950);
        }
    }
}

void copy(char *input_buf, byte current_dir){
    int i, j;
    char file_source[16];
    char file_dest[16];
    byte buffer[8192];
    enum fs_retcode return_code;
    struct file_metadata metadata;
    i = 2;
    while (input_buf[i] == ' ') {
        i++;
    }
    i++;
    if (input_buf[i] == '\0') {
        printString("File asal dan tujuan tidak diberikan!\nContoh: cp file1.txt file2.txt\n");
        return;
    }
    
    i++;
    j = 0;
    while (input_buf[i] != ' ' && input_buf[i] != '\0') {
        file_source[j] = input_buf[i];
        i++;
        j++;
    }

    if (input_buf[i] == '\0') {
        printString("File tujuan tidak diberikan!\nContoh: cp file1.txt file2.txt\n");
        return;
    }

    i++;
    j = 0;
    while (input_buf[i] != ' ' && input_buf[i] != '\0') {
        file_dest[j] = input_buf[i];
        i++;
        j++;
    }

    if (strlen(file_dest) == strlen(file_source)) {
        if (strcmp(file_dest, file_source)) {
            printString("Nama file asal sama dengan nama file tujuan!\n");
            return;
        }
    }

    metadata.buffer = buffer;
    metadata.node_name = file_source;
    metadata.parent_index = current_dir;

    read(&metadata, &return_code);
    if (return_code != 0) {
        printString("Pembacaan file asal gagal dengan kode error ");
        printNumber(return_code);
        printString("\n");
        return;
    }

    metadata.node_name = file_dest;
    // parent_index, buffer, dan filesize sudah ada

    write(&metadata, &return_code);

    if (return_code != 0) {
        printString("Penulisan file tujuan gagal dengan kode error ");
        printNumber(return_code);
        printString("\n");
        return;
    }
}