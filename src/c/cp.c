#include "header/fileio.h"
#include "header/filesystem.h"
#include "header/message.h"
#include "header/program.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/utils.h"
#include "string.h"

int main() {
    struct message msg;
    int i, j;
    char* arg1;
    char* arg2;
    char* copy_name;
    char dest_name[16];
    byte buffer[8192];
    enum fs_retcode return_code;
    struct file_metadata metadata;

    getMessage(&msg);
    copy_name = "_copy\0";
    return_code = 0;
    arg1 = msg.arg1;
    arg2 = msg.arg2;
    // tidak ada nama file asal
    if (arg1[0] == 0) {
        puts("cp: Trainer-chan!! File asal tidak diberikan!\n");
        exit();
    }

    if (strlen(arg1) == strlen(arg2)) {
        if (strcmp(arg1, arg2)) {
            puts("mv: Trainer-chan!! Nama file asal sama dengan nama file tujuan!\n");
            exit();
        }
    }

    // clear buffer
    for (i = 0; i < 16; i++) dest_name[i] = '\0';

    metadata.buffer = buffer;
    metadata.node_name = arg1;
    metadata.parent_index = msg.current_directory;

    read(&metadata, &return_code);

    if (return_code != 0) {
        puts("mv: Trainer-chan!! Baca file asal gagal dengan kode error ");
        putsNumber(return_code);
        puts("\n");
        exit();
    }

    if (arg2[0] == 0) {
        strcpy(dest_name, arg1);
        i = 0;
        j = 0;
        while (dest_name[i] != '\0') i++;
        while (i < 16 && j < 6) {
            dest_name[i] = copy_name[j];
            i++;
            j++;
        }
    } else {
        strcpy(dest_name, arg2);
    }
    metadata.node_name = dest_name;

    // parent_index, buffer, dan filesize sudah ada
    write(&metadata, &return_code);

    if (return_code != 0) {
        puts("mv: Trainer-chan!! Tulis file tujuan gagal dengan kode error ");
        putsNumber(return_code);
        puts("\n");
        exit();
    }

    exit();
}
