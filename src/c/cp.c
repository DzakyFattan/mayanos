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
    char *file_source;
    char *copy_name = "_copy\0";
    char *input_buf;
    char *file_dest;
    byte buffer[8192];
    byte current_dir;
    enum fs_retcode return_code;
    struct file_metadata metadata;
    struct message msg;
    struct message msg_next;

    getMessage(&msg, getCurrentSegment());
    getMessage(&msg_next, msg.next_program_segment);
    msg_next.current_directory = msg.current_directory;
    setMessage(&msg_next, msg.next_program_segment);

    current_dir = msg.current_directory;

    return_code = 0;
    file_source = msg.arg1;
    file_dest = msg.arg2;

    i = 2;
    if (file_source[0] == '\0') {
        puts("cp: Trainer-chan!! File asal tidak diberikan!\n");
        exit();
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
            puts("cp: Trainer-chan!! Nama file asal sama dengan nama file tujuan!\n");
            exit();
        }
    }

    metadata.buffer = buffer;
    metadata.node_name = file_source;
    metadata.parent_index = current_dir;

    read(&metadata, &return_code);

    if (return_code != 0) {
        puts("cp: Trainer-chan!! Baca file asal gagal dengan kode error ");
        putsNumber(return_code);
        puts("\n");
        exit();
    }

    metadata.node_name = file_dest;

    // parent_index, buffer, dan filesize sudah ada
    write(&metadata, &return_code);

    if (return_code != 0) {
        puts("cp: Trainer-chan!! Tulis file tujuan gagal dengan kode error ");
        putsNumber(return_code);
        puts("\n");
        exit();
    }

    exit();
}
