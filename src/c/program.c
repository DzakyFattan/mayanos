#include "header/program.h"

void exec(struct file_metadata *metadata, int segment) {
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
    }
    else
        printString("exec: Trainer-chan!! file tidak ditemukan!!\r\n");
}

void exit() {
    struct file_metadata shell;
    shell.node_name = "shell";
    shell.parent_index = 0;
    exec(&shell, 0x4000);
}