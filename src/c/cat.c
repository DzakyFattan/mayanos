#include "header/fileio.h"
#include "header/filesystem.h"
#include "header/message.h"
#include "header/program.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/utils.h"

int main() {
    byte current_dir;
    char lines[128][80];
    int line_track = 0;
    int i = 0;
    int j = 0;

    int input, num;
    int upLimit, downLimit;
    int scrollAmount = 0;

    struct file_metadata metadata;
    struct message msg;
    struct message msg_next;
    enum fs_retcode retcode;
    char *input_buf;
    char *file_name;
    byte metadata_buf[8192];

    getMessage(&msg, getCurrentSegment());
    getMessage(&msg_next, msg.next_program_segment);
    msg_next.current_directory = msg.current_directory;
    setMessage(&msg_next, msg.next_program_segment);
    file_name = msg.arg1;

    current_dir = msg.current_directory;

    metadata.buffer = metadata_buf;
    metadata.node_name = file_name;
    metadata.parent_index = current_dir;

    read(&metadata, &retcode);

    if (retcode == FS_R_NODE_NOT_FOUND) {
        puts("cat: Trainer-chan!! File tidak ditemukan!\r\n");
        exit();
    }

    if (retcode == FS_R_TYPE_IS_FOLDER) {
        puts("cat: Trainer-chan!! Kamu memasukkan input Folder!\r\n");
        exit();
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
        puts(lines[i]);
        puts("\n");
        i++;
    }

    // line track starts at 0
    upLimit = line_track - 21; // top most line
    downLimit = line_track;    // bottom most line, the last line of the text

    // print file info
    puts("========================\nNama File: ");
    puts(file_name);
    puts("\n");

    // scroll
    puts("Sudah selesai bacanya? Tekan ESC untuk keluar ya, Trainer-chan!! ^^");
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
                    puts(lines[i]);
                    puts("\n");
                }
            } else if (input == 0x50 && downLimit < line_track) {
                upLimit++;
                downLimit++;
                interrupt(0x10, 0x0601, 0x0, 0x0000, 0x1549);
                interrupt(0x10, 0x0200, 0x0, 0x0, 0x1500);
                puts(lines[downLimit]);
            }
            continue;

        } else if (num == 27) {
            interrupt(0x10, 0x0200, 0x0, 0x0, 0x1849);
            puts("\n");
            break;
        }
    }

    exit();
}

int div(int a, int b) {
    return a / b;
}

int mod(int a, int n) {
    return (a - n * (a / n));
}