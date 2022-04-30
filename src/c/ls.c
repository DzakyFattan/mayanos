#include "header/fileio.h"
#include "header/filesystem.h"
#include "header/message.h"
#include "header/program.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/utils.h"

int main() {
    char file_entry[64][16];
    char folder_entry[64][16];
    int strcap = 0;
    int len = 0;
    int file_e_counter = 0;
    int folder_e_counter = 0;
    int i = 0;
    byte current_dir;

    struct message msg;
    struct message msg_next;
    struct node_filesystem node_fs_buffer;

    getMessage(&msg, getCurrentSegment());
    getMessage(&msg_next, msg.next_program_segment);
    msg_next.current_directory = msg.current_directory;
    setMessage(&msg_next, msg.next_program_segment);
    current_dir = msg.current_directory;

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
            puts("\n");
            strcap = len;
        }
        puts(folder_entry[i]);
        puts(" ");
        i++;
    }

    i = 0;
    while (i < file_e_counter) {
        len = strlen(file_entry[i]);
        strcap += len + 1;
        if (strcap > 64) {
            puts("\n");
            strcap = len;
        }
        puts(file_entry[i]);
        puts(" ");
        i++;
    }

    if (file_e_counter == 0 && folder_e_counter == 0) {
        puts("ls: Trainer-chan!! Tidak ada File dan Folder apapun di sini!\r\n");
    } else {
        puts("\n");
        for (i = 0; i < 64; i++) {
            strclr(file_entry[i]);
            strclr(folder_entry[i]);
        }
    }

    exit();
}