#include "header/fileio.h"
#include "header/filesystem.h"
#include "header/message.h"
#include "header/program.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/utils.h"

int main() {
    byte current_dir;
    struct file_metadata metadata;
    struct message msg;
    enum fs_retcode retcode;
    char *input_buf;
    char *folder_name;
    char *copy_name = "_copy\0";
    int i;
    int input_len;
    int len;

    getMessage(&msg);
    current_dir = msg.current_directory;
    input_buf = msg.arg1;

    if (strcmp(input_buf, "mkdir")) {
        puts("cp: Trainer-chan!! Nama folder tidak diberikan!\n");
        exit();
    }

    strcpy(folder_name, input_buf + 6);
    input_len = strlen(input_buf);
    len = input_len - 6;

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

    exit();
}