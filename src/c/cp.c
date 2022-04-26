#include "header/fileio.h"
#include "header/filesystem.h"
#include "header/message.h"
#include "header/program.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/utils.h"
#include "string.h"

int main() {
    int i, j;
    char file_source[16];
    char *copy_name = "_copy\0";
    char *input_buf;
    char file_dest[16];
    byte buffer[4096];
    byte current_dir;
    enum fs_retcode return_code;
    struct file_metadata metadata;
    struct message msg;

    getMessage(&msg);
    current_dir = msg.current_directory;
    input_buf = msg.arg1;

    return_code = 0;
    for (i = 0; i < 16; i++) {
        file_source[i] = '\0';
        file_dest[i] = '\0';
    }

    i = 2;
    while (input_buf[i] == ' ' && input_buf[i] != '\0')
        i++;
    if (input_buf[i] == '\0') {
        puts("cp: Trainer-chan!! File asal tidak diberikan!\n");
        exit();
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
            puts("mv: Trainer-chan!! Nama file asal sama dengan nama file tujuan!\n");
            exit();
        }
    }

    metadata.buffer = buffer;
    metadata.node_name = file_source;
    metadata.parent_index = current_dir;

    read(&metadata, &return_code);

    if (return_code != 0) {
        puts("mv: Trainer-chan!! Baca file asal gagal dengan kode error ");
        putsNumber(return_code);
        puts("\n");
        exit();
    }

    metadata.node_name = file_dest;

    // parent_index, buffer, dan filesize sudah ada
    write(&metadata, &return_code);

    if (return_code != 0) {
        puts("Tulis file tujuan gagal dengan kode error ");
        putsNumber(return_code);
        puts("\n");
        exit();
    }

    exit();
}
