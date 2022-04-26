#include "header/fileio.h"
#include "header/filesystem.h"
#include "header/message.h"
#include "header/program.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/utils.h"

int main() {
    int i, j;
    bool found;
    byte parent_idx;
    byte current_dir;
    struct node_filesystem node_fs_buffer;
    struct message msg;
    char first_arg[19];
    char second_arg[19];
    char *input_buf;

    getMessage(&msg);
    current_dir = msg.current_directory;
    input_buf = msg.arg1;

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
        puts("mv: Trainer-chan!! File/folder asal tidak ditemukan!\n");
        exit();
    }

    if (second_arg[0] == '/') {
        // Kasus mengubah file/folder ke root
        strcpy(node_fs_buffer.nodes[i].name, second_arg + 1);
        node_fs_buffer.nodes[i].parent_node_index = FS_NODE_P_IDX_ROOT;
    } else if (second_arg[0] == '.' && second_arg[1] == '.' && second_arg[2] == '/') {
        // Kasus mengubah folder dari folder/file ke parent
        parent_idx = node_fs_buffer.nodes[i].parent_node_index;
        if (parent_idx == FS_NODE_P_IDX_ROOT) {
            puts("mv: Trainer-chan!! File/folder sudah berada pada root!\n");
            exit();
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
            puts("mv: Trainer-chan!! Folder tujuan tidak ditemukan!\n");
            exit();
        }

        node_fs_buffer.nodes[i].parent_node_index = j;
    }

    writeSector(&node_fs_buffer.nodes[0], 0x101);
    writeSector(&node_fs_buffer.nodes[32], 0x102);

    exit();
}