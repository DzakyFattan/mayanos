// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    struct file_metadata meta;
    byte root[512];
    root[0] = 0xFF;
    fillMap();
    makeInterrupt21();
    interrupt(0x10, 0x0010, 0x0, 0x0, 0x0);
    // interrupt(0x10, 0x0003, 0x0, 0x0, 0x0);
    // clearScreen();
    interrupt(0x21, 0xB, "Pin Pon!! This is MayanOS!! >//<\n", BROWN, 0x0);
    interrupt(0x10, 0x0E0A, 0x0, 0x0, 0x0);
    interrupt(0x21, 0xB, ":::=======  :::====  ::: === :::====  :::= === :::====  :::=== \n", YELLOW, 0x0);
    interrupt(0x21, 0xB, "::: === === :::  === ::: === :::  === :::===== :::  === :::    \n", YELLOW, 0x0);
    interrupt(0x21, 0xB, "=== === === ========  =====  ======== ======== ===  ===  ===== \n", YELLOW, 0x0);
    interrupt(0x21, 0xB, "===     === ===  ===   ===   ===  === === ==== ===  ===     ===\n", YELLOW, 0x0);
    interrupt(0x21, 0xB, "===     === ===  ===   ===   ===  === ===  ===  ======  ====== \n", YELLOW, 0x0);
    interrupt(0x21, 0xB, "\nMaya siap membantu Trainer-chan, You Copy?! ( ^ w ^)7\r\n", BROWN, 0x0);

    writeSector(root, 0x104);

    meta.node_name = "shell";
    meta.parent_index = 0;
    executeProgram(&meta, 0x4000);

    shell();
    while (true)
        ;
}

int handleInterrupt21(int AX, int BX, int CX, int DX) {
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
    case 0x6:
        AX = strlen(BX);
        break;
    case 0x7:
        AX = strcmp(BX, CX);
        break;
    case 0x8:
        strcpy(BX, CX);
        break;
    case 0x9:
        strclr(BX);
        break;
    case 0xA:
        clearScreen();
        break;
    case 0xB:
        printColor(BX, CX);
        break;
    case 0xC:
        printCWD(BX);
        break;
    case 0xD:
        printNumber(BX);
        break;
    case 0xE:
        exit();
        break;
    case 0xF:
        executeProgram(BX, CX);
        break;
    default:
        printString("Invalid Interrupt");
    }
}

void executeProgram(struct file_metadata *metadata, int segment) {
    enum fs_retcode fs_ret;
    byte buf[8192];
    metadata->buffer = buf;
    read(metadata, &fs_ret);
    if (fs_ret == FS_SUCCESS) {
        int i = 0;
        for (i = 0; i < 8192; i++) {
            if (i < metadata->filesize)
                putInMemory(segment, i, metadata->buffer[i]);
            else
                putInMemory(segment, i, 0x00);
        }
        launchProgram(segment);
    } else
        printString("execute: Trainer-chan!! file tidak ditemukan!!\r\n");
}

void exit() {
    struct file_metadata shell;
    shell.node_name = "shell";
    shell.parent_index = 0;
    executeProgram(&shell, 0x4000);
}

void clearScreen() {
    interrupt(0x10, 0x0700, 0x0700, 0x0, SCREEN_WIDTH + SCREEN_HEIGHT);
    interrupt(0x10, 0x0200, 0x0, 0x0, 0x0);
}

void printString(char *string) {
    for (; *string != '\0'; string++) {
        if (*string == '\r' || *string == '\n') {
            interrupt(0x10, 0x0E0A, 0x0, 0x0, 0x0);
            interrupt(0x10, 0x0E0D, 0x0, 0x0, 0x0);
        } else {
            int AX = 0x0E00 + *string;
            interrupt(0x10, AX, 0x0007, 0x0, 0x0);
        }
    }
}

void printColor(char *string, int color) {
    for (; *string != '\0'; string++) {
        if (*string == '\r' || *string == '\n') {
            interrupt(0x10, 0x0E0A, 0x0, 0x0, 0x0);
            interrupt(0x10, 0x0E0D, 0x0, 0x0, 0x0);
        } else {
            int AX = 0x0E00 + *string;
            interrupt(0x10, AX, 0x0 + color, 0x0, 0x0);
        }
    }
}

void printNumber(int number) {
    int arr[10];
    int digit;
    int j, i = 0;

    while (number != 0) {
        digit = mod(number, 10);

        arr[i] = digit;
        i++;
        number = div(number, 10);
    }

    for (j = i - 1; j >= 0; j--) {
        int AX = 0x0E00 + arr[j] + 48;
        interrupt(0x10, AX, 0x0007, 0x0, 0x0);
    }
}

void readString(char *string) {
    int AX, num, input;
    int cur = 0;
    int j = 0;
    int currentBufLength = 0; // act as where to put null string
    while (true) {
        input = interrupt(0x16, 0x00, 0x00, 0x00, 0x00);
        num = mod(input, 0x100);
        input = div(input, 0x100);

        if (num == 0) {
            // handling arrow keys
            if (input == 0x48 && cur > 0) {
                for (j = 0; j < cur; j++) {
                    interrupt(0x10, 0x0E08, 0x0, 0x0, 0x0);
                }
                cur = 0;
            } else if (input == 0x50 && cur < currentBufLength) {
                for (j = cur; j < currentBufLength; j++) {
                    interrupt(0x10, 0x0E00 + string[j], 0x0007, 0x0, 0x0);
                }
                cur = currentBufLength;
            } else if (input == 0x4B && cur > 0) {
                interrupt(0x10, 0x0E08, 0x0, 0x0, 0x0);
                cur--;
            } else if (input == 0x4D && cur < currentBufLength) {
                interrupt(0x10, 0x0E00 + string[cur], 0x0007, 0x0, 0x0);
                cur++;
            }
            continue;
        } else if (num == 13) {
            // enter
            interrupt(0x10, 0x0E0A, 0x0, 0x0, 0x0);
            interrupt(0x10, 0x0E0D, 0x0, 0x0, 0x0);
            break;
        } else if (num == 8) {
            // backspace
            if (cur > 0) {
                interrupt(0x10, 0x0E08, 0x0, 0x0, 0x0);
                cur--;
                currentBufLength--;
                for (j = cur; j < currentBufLength; j++) {
                    interrupt(0x10, 0x0E00 + string[j], 0x0007, 0x0, 0x0);
                    string[j] = string[j + 1];
                }
                interrupt(0x10, 0x0E00 + ' ', 0x0007, 0x0, 0x0);
                for (j = currentBufLength; j >= cur; j--) {
                    interrupt(0x10, 0x0E08, 0x0, 0x0, 0x0);
                }
                string[j + 1] = '\0';
            }
            continue;

        } else if (num == 27) {
            // escape
            clear(string, cur + 1);
            for (j = 0; j < currentBufLength; j++) {
                interrupt(0x10, 0x0E08, 0x0, 0x0, 0x0);
                interrupt(0x10, 0x0E00 + ' ', 0x0007, 0x0, 0x0);
                interrupt(0x10, 0x0E08, 0x0, 0x0, 0x0);
            }
            currentBufLength = 0;
            cur = 0;
            continue;
        }

        // print character on the screen
        AX = 0x0E00 + num;
        interrupt(0x10, AX, 0x0007, 0x0, 0x0);
        cur++;
        currentBufLength++;

        // shift characters on the right if exists
        for (j = cur; j < currentBufLength; j++) {
            interrupt(0x10, 0x0E00 + string[j - 1], 0x0007, 0x0, 0x0);
        }
        for (j = currentBufLength - 1; j >= cur; j--) {
            string[j] = string[j - 1];
            interrupt(0x10, 0x0E08, 0x0, 0x0, 0x0);
        }
        string[cur - 1] = num;
    }
    string[currentBufLength] = '\0';
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
    for (i = 0; i <= 31; i++) {
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
    for (i = 0; i < 512; i++) {
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
    // 2. Lakukan iterasi proses berikut, i = 0..15
    // 3. Baca byte entry sector untuk mendapatkan sector number partisi file
    // 4. Jika byte bernilai 0, selesaikan iterasi
    // 5. Jika byte valid, lakukan readSector()
    //    dan masukkan kedalam buffer yang disediakan pada metadata
    // 6. Lompat ke iterasi selanjutnya hingga iterasi selesai
    memcpy(sector_nums_buffer, sector_fs_buffer.sector_list[sector_entry_index].sector_numbers, 16);
    metadata->filesize = 0;
    for (i = 0; i < 16; i++) {
        if (sector_nums_buffer[i] == 0) {
            break;
        }
        readSector(&(metadata->buffer[i * 512]), sector_nums_buffer[i]);
        metadata->filesize += 512;
    }

    // 7. Tulis retcode FS_SUCCESS pada akhir proses pembacaan.
    *return_code = FS_SUCCESS;
}

void shell() {
    char input_buf[64];
    char path_str[128];
    char command[16];
    int i = 0;
    int j = 0;
    int scrollLine;

    byte current_dir = FS_NODE_P_IDX_ROOT;

    while (true) {
        printColor("MayanOS:", BROWN);
        printCWD(current_dir);
        printString("$ ");

        readString(input_buf);
        while (input_buf[i] != ' ' && input_buf[i] != '\0') {
            command[i] = input_buf[i];
            i++;
        }
        command[i] = '\0';
        i++;

        if (strcmp(command, "clear")) {
            clearScreen();
        } else if (strcmp(command, "exit")) {
            printString("Trainer-chan mau pergi? :( Kapan-kapan main ke sini lagi ya! ( ^ w ^) <3\r\n");
            break;
        } else if (strcmp(command, "ls")) {
            ls(current_dir);
        } else if (strcmp(command, "cd")) {
            cd(input_buf, &current_dir);
        } else if (strcmp(command, "cp")) {
            copy(input_buf, current_dir);
        } else if (strcmp(command, "mkdir")) {
            mkdir(input_buf, current_dir);
        } else if (strcmp(command, "rm")) {
            printString("rrmad\n");
        } else if (strcmp(command, "cat")) {
            cat(input_buf, current_dir);
        } else if (strcmp(command, "mv")) {
            move(input_buf, current_dir);
        } else if (strcmp(input_buf, "shell")) {
            struct file_metadata shell;
            shell.node_name = "shell";
            shell.parent_index = 0;
            executeProgram(&shell, 0x4000);
        } else if (strcmp(input_buf, "Aku sayang sama Maya-chin")) {
            printColor("Hehe, Maya juga sayang sama Trainer-chan ( ^ w ^) <3<3<3\r\n", BROWN);
        } else {
            printColor("Maya ngga ngerti perintah Trainer-chan (# -_-)\r\n", BROWN);
        }
        strclr(input_buf);
        strclr(path_str);
        strclr(command);
        i = 0;
        j = 0;
    }
}

void printCWD(byte current_dir) {
    char temp[64][16];
    char path_str[128];

    int i = 0;
    int j = 0;

    bool found = false;
    struct node_filesystem node_fs_buffer;

    readSector(&node_fs_buffer.nodes[0], FS_NODE_SECTOR_NUMBER);
    readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 0x1);

    if (current_dir == FS_NODE_P_IDX_ROOT) {
        path_str[0] = '/';
        path_str[1] = '\0';
        printColor(path_str, YELLOW);
        return;
    }

    strcpy(path_str, "/");
    while (current_dir != FS_NODE_P_IDX_ROOT && i < 64) {
        strcpy(temp[i], node_fs_buffer.nodes[current_dir].name);
        current_dir = node_fs_buffer.nodes[current_dir].parent_node_index;
        i++;
    }

    for (i = i - 1; i >= 0; i--) {
        strcpy(&(path_str[j]), temp[i]);
        j += strlen(temp[i]);
        path_str[j] = '/';
        j++;
    }
    path_str[j] = '\0';
    printColor(path_str, YELLOW);
}

void cd(char *path_str, byte *current_dir) {
    char path[64];
    char temp[16];
    int tempdst = 0;
    int path_length;
    int i = 0;
    int j = 0;
    int k = 0;

    struct node_filesystem node_fs_buffer;

    readSector(&node_fs_buffer.nodes[0], FS_NODE_SECTOR_NUMBER);
    readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 0x1);

    strcpy(path, path_str + 3);
    path_length = strlen(path);

    tempdst = *current_dir;
    if (strcmp(path, "..")) {
        if (*current_dir == FS_NODE_P_IDX_ROOT) {
            printString("cd: Trainer-chan!! Ngga bisa balik dari root!\r\n");
            return;
        }
        while (i < 64) {
            if (*current_dir == i) {
                *current_dir = node_fs_buffer.nodes[i].parent_node_index;
                return;
            }
            i++;
        }

    } else if (strcmp(path, "/")) {
        *current_dir = FS_NODE_P_IDX_ROOT;
    } else {
        while (i < path_length) {
            while (path[i] != '/' && path[i] != '\0') {
                temp[j] = path[i];
                i++;
                j++;
            }
            temp[j] = '\0';
            while (k < 64) {
                if (strcmp(temp, node_fs_buffer.nodes[k].name) && node_fs_buffer.nodes[k].sector_entry_index == 0xFF && node_fs_buffer.nodes[k].parent_node_index == tempdst) {
                    tempdst = k;

                    break;
                }
                k++;
            }
            if (k == 64) {
                printString("cd: Trainer-chan!! Direktori tidak ditemukan!\r\n");
                strclr(temp);
                strclr(path);
                return;
            }
            strclr(temp);
            j = 0;
            k = 0;
            i++;
        }
        if (i == 0) {
            printString("cd: Trainer-chan!! Argumennya invalid!\r\n");
            strclr(temp);
            strclr(path);
            return;
        }
        *current_dir = tempdst;
    }
}

void ls(byte current_dir) {
    char file_entry[64][16];
    char folder_entry[64][16];
    int strcap = 0;
    int len = 0;
    int file_e_counter = 0;
    int folder_e_counter = 0;
    int i = 0;

    struct node_filesystem node_fs_buffer;

    // get file system node
    readSector(&node_fs_buffer.nodes[0], FS_NODE_SECTOR_NUMBER);
    readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 0x1);

    // put into either entries
    while (i < 64) {
        if (node_fs_buffer.nodes[i].parent_node_index == current_dir) {
            if (node_fs_buffer.nodes[i].sector_entry_index == 0xFF) {
                strcpy(folder_entry[folder_e_counter], node_fs_buffer.nodes[i].name);
                folder_e_counter++;
            } else {
                strcpy(file_entry[file_e_counter], node_fs_buffer.nodes[i].name);
                file_e_counter++;
            }
        }
        i++;
    }

    /// print
    i = 0;
    while (i < folder_e_counter) {
        len = strlen(folder_entry[i]);
        strcap += len + 1;
        if (strcap > 64) {
            printString("\n");
            strcap = len;
        }
        printString(folder_entry[i]);
        printString(" ");
        i++;
    }

    i = 0;
    while (i < file_e_counter) {
        len = strlen(file_entry[i]);
        strcap += len + 1;
        if (strcap > 64) {
            printString("\n");
            strcap = len;
        }
        printString(file_entry[i]);
        printString(" ");
        i++;
    }

    if (file_e_counter == 0 && folder_e_counter == 0) {
        printString("ls: Trainer-chan!! Tidak ada File dan Folder apapun di sini!\r\n");
    } else {
        printString("\n");
        for (i = 0; i < 64; i++) {
            strclr(file_entry[i]);
            strclr(folder_entry[i]);
        }
    }
}

void mkdir(char *input_buf, byte current_dir) {
    struct file_metadata metadata;
    enum fs_retcode retcode;
    char *folder_name = input_buf + 6;
    char *copy_name = "_copy\0";
    int i;
    int input_len = strlen(input_buf);
    int len = input_len - 6;

    folder_name[len] = '\0';
    metadata.node_name = folder_name;
    metadata.parent_index = current_dir;
    metadata.filesize = 0;

    write(&metadata, &retcode);

    while (retcode == FS_W_FILE_ALREADY_EXIST) {
        for (i = 0; i < 6; i++, len++) {
            folder_name[len] = copy_name[i];
        }
        folder_name[strlen(folder_name)] = '\0';
        len = strlen(folder_name);
        metadata.node_name = folder_name;
        write(&metadata, &retcode);
    }
}

void cat(char *input_buf, byte current_dir) {
    char lines[128][80];
    int line_track = 0;
    int i = 0;
    int j = 0;

    int input, num;
    int upLimit, downLimit;
    int scrollAmount = 0;

    struct file_metadata metadata;
    enum fs_retcode retcode;
    char *file_name = input_buf + 4;
    byte metadata_buf[8192];

    metadata.buffer = metadata_buf;
    metadata.node_name = file_name;
    metadata.parent_index = current_dir;

    read(&metadata, &retcode);

    if (retcode == FS_R_NODE_NOT_FOUND) {
        printString("cat:mv: Trainer-chan!! File tidak ditemukan!\r\n");
        return;
    }

    if (retcode == FS_R_TYPE_IS_FOLDER) {
        printString("cat: Trainer-chan!! Kamu memasukkan input Folder!\r\n");
        return;
    }

    // read metadata.buffer into lines
    while (i < metadata.filesize) {
        if (metadata.buffer[i] == '\0') {
            lines[line_track][j] = '\0';
            break;
        } else if (j >= 79 || metadata.buffer[i] == '\r' || metadata.buffer[i] == '\n') {
            if (metadata.buffer[i] == '\r') {
                i++;
            }
            lines[line_track][j] = '\0';
            line_track++;
            j = 0;
        } else {
            lines[line_track][j] = metadata.buffer[i];
            j++;
        }
        i++;
    }

    // print lines
    i = 0;
    while (i <= line_track) {
        printString(lines[i]);
        printString("\n");
        i++;
    }

    // line track starts at 0
    upLimit = line_track - 21; // top most line
    downLimit = line_track;    // bottom most line, the last line of the text

    // print file info
    printString("========================\nNama File: ");
    printString(file_name);
    printString("\n");

    // scroll
    printColor("Sudah selesai bacanya? Tekan ESC untuk keluar ya, Trainer-chan!! ^^", BROWN);
    while (true) {
        input = interrupt(0x16, 0x00, 0x00, 0x00, 0x00);
        num = mod(input, 0x100);
        input = div(input, 0x100);
        if (num == 0) {
            if (input == 0x48 && upLimit > 0) {
                upLimit--;
                downLimit--;
                interrupt(0x10, 0x0701, 0x0, 0x0000, 0x1549);
                interrupt(0x10, 0x0200, 0x0, 0x0, 0x0000);
                for (i = upLimit; i <= upLimit + 21; i++) {
                    interrupt(0x10, 0x0A00 + ' ', 0x0, 80, 0x0);
                    interrupt(0x10, 0x0E0D, 0x0, 0x0, 0x0);
                    printString(lines[i]);
                    printString("\n");
                }
            } else if (input == 0x50 && downLimit < line_track) {
                upLimit++;
                downLimit++;
                interrupt(0x10, 0x0601, 0x0, 0x0000, 0x1549);
                interrupt(0x10, 0x0200, 0x0, 0x0, 0x1500);
                printString(lines[downLimit]);
            }
            continue;

        } else if (num == 27) {
            interrupt(0x10, 0x0200, 0x0, 0x0, 0x1849);
            printString("\n");
            break;
        }
    }
}

void copy(char *input_buf, byte current_dir) {
    int i, j;
    char file_source[16];
    char *copy_name = "_copy\0";
    char file_dest[16];
    byte buffer[4096];
    enum fs_retcode return_code;
    struct file_metadata metadata;

    return_code = 0;
    for (i = 0; i < 16; i++) {
        file_source[i] = '\0';
        file_dest[i] = '\0';
    }

    i = 2;
    while (input_buf[i] == ' ' && input_buf[i] != '\0')
        i++;
    if (input_buf[i] == '\0') {
        printString("cp: Trainer-chan!! File asal tidak diberikan!\n");
        return;
    }

    j = 0;
    while (input_buf[i] != ' ' && input_buf[i] != '\0') {
        file_source[j] = input_buf[i];
        i++;
        j++;
    }

    while (input_buf[i] == ' ')
        i++;

    j = 0;
    while (input_buf[i] != '\0' && input_buf[i] != ' ') {
        file_dest[j] = input_buf[i];
        i++;
        j++;
    }

    if (strlen(file_dest) == 0) {
        for (i = 0; i < strlen(file_source); i++) {
            file_dest[i] = file_source[i];
        }
        j = 0;
        while (i < 16 && j < 6) {
            file_dest[i] = copy_name[j];
            i++;
            j++;
        }

        file_dest[15] = '\0';
    }

    if (strlen(file_dest) == strlen(file_source)) {
        if (strcmp(file_dest, file_source)) {
            printString("mv: Trainer-chan!! Nama file asal sama dengan nama file tujuan!\n");
            return;
        }
    }

    metadata.buffer = buffer;
    metadata.node_name = file_source;
    metadata.parent_index = current_dir;

    read(&metadata, &return_code);

    if (return_code != 0) {
        printString("mv: Trainer-chan!! Baca file asal gagal dengan kode error ");
        printNumber(return_code);
        printString("\n");
        return;
    }

    metadata.node_name = file_dest;

    // parent_index, buffer, dan filesize sudah ada
    write(&metadata, &return_code);

    if (return_code != 0) {
        printString("Tulis file tujuan gagal dengan kode error ");
        printNumber(return_code);
        printString("\n");
        return;
    }
}

void move(char *input_buf, byte current_dir) {
    int i, j;
    bool found;
    byte parent_idx;
    struct node_filesystem node_fs_buffer;
    char first_arg[19];
    char second_arg[19];

    for (i = 0; i < 19; i++) {
        first_arg[i] = '\0';
        second_arg[i] = '\0';
    }

    i = 3;
    j = 0;
    while (input_buf[i] != ' ') {
        first_arg[j] = input_buf[i];
        i++;
        j++;
    }

    i++;
    j = 0;
    while (input_buf[i] != '\0') {
        second_arg[j] = input_buf[i];
        i++;
        j++;
    }

    // get file system node
    readSector(&node_fs_buffer.nodes[0], FS_NODE_SECTOR_NUMBER);
    readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 0x1);

    // Mencari file/folder source
    i = 0;
    found = false;
    while (i < 64 && !found) {
        if (node_fs_buffer.nodes[i].parent_node_index == current_dir) {
            if (strcmp(node_fs_buffer.nodes[i].name, first_arg)) {
                found = true;
            }
        }
        if (!found)
            i++;
    }

    if (!found) {
        printString("mv: Trainer-chan!! File/folder asal tidak ditemukan!\n");
        return;
    }

    if (second_arg[0] == '/') {
        // Kasus mengubah file/folder ke root
        strcpy(node_fs_buffer.nodes[i].name, second_arg + 1);
        node_fs_buffer.nodes[i].parent_node_index = FS_NODE_P_IDX_ROOT;
    } else if (second_arg[0] == '.' && second_arg[1] == '.' && second_arg[2] == '/') {
        // Kasus mengubah folder dari folder/file ke parent
        parent_idx = node_fs_buffer.nodes[i].parent_node_index;
        if (parent_idx == FS_NODE_P_IDX_ROOT) {
            printString("mv: Trainer-chan!! File/folder sudah berada pada root!\n");
            return;
        }

        strcpy(node_fs_buffer.nodes[i].name, second_arg + 3);
        node_fs_buffer.nodes[i].parent_node_index = node_fs_buffer.nodes[parent_idx].parent_node_index;
    } else {
        // Kasus memasukkan file/folder ke dalam suatu folder
        j = 0;
        found = false;
        while (j < 64 && !found) {
            if (node_fs_buffer.nodes[j].parent_node_index == current_dir) {
                if (node_fs_buffer.nodes[j].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
                    if (strcmp(node_fs_buffer.nodes[j].name, second_arg)) {
                        found = true;
                    }
                }
            }
            if (!found)
                j++;
        }
        if (!found) {
            printString("mv: Trainer-chan!! Folder tujuan tidak ditemukan!\n");
            return;
        }

        node_fs_buffer.nodes[i].parent_node_index = j;
    }

    writeSector(&node_fs_buffer.nodes[0], 0x101);
    writeSector(&node_fs_buffer.nodes[32], 0x102);
}

void scrollController(int lines) {
    int i;
    if (lines < 256) {
        for (i = 0; i < lines; i++) {
            interrupt(0x10, 0x0600 + lines, 0x0000, 0x0, SCREEN_WIDTH + SCREEN_HEIGHT);
        }
    }
}