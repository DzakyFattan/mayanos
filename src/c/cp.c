#include "header/fileio.h"
#include "header/filesystem.h"
#include "header/program.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/utils.h"
#include "header/message.h"

int main() {
    struct message msg;
    int i, j;

    char* arg1;
    char* arg2;
    char* arg3;
    
    char *copy_name;
    byte buffer[8192];
    enum fs_retcode return_code;
    struct file_metadata metadata;

    getMessage(&msg);
    copy_name = "_copy\0";
    return_code = 0;
    arg1 = msg.arg1;
    arg2 = msg.arg2;
    arg3 = msg.arg3;
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

    metadata.buffer = buffer;
    metadata.node_name = arg1;
    metadata.parent_index = msg.current_directory;

    read(&metadata, &return_code);

    if (return_code != 0) {
        puts("mv: Trainer-chan!! Baca file asal gagal dengan kode error ");
        putsNumber(return_code);
        puts("\n");
        return;
    }

    if (arg2[0] == 0) {
        i = 0;
        j = 0;
        while (arg1[i] != 0) i++;
        while (i < 16 && j < 4) {
            arg1[i] = copy_name[j];
            i++; j++;
        }
        metadata.node_name = arg1;
    }

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
