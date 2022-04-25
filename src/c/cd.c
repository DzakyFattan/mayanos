#include "header/fileio.h"
#include "header/filesystem.h"
#include "header/message.h"
#include "header/program.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/utils.h"

int main() {
    char *path_str;
    byte *current_dir;
    char path[64];
    char temp[16];
    int tempdst = 0;
    int path_length;
    int i = 0;
    int j = 0;
    int k = 0;

    struct message msg;
    struct node_filesystem node_fs_buffer;

    getMessage(&msg);
    current_dir = &(msg.current_directory);
    path_str = msg.arg1;

    readSector(&node_fs_buffer.nodes[0], FS_NODE_SECTOR_NUMBER);
    readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 0x1);

    strcpy(path, path_str + 3);
    // path_length = strlen(path);
    for (path_length = 0; path[path_length] != 0; path_length++)
        ;

    tempdst = *current_dir;
    if (strcmp(path, "..")) {
        if (*current_dir == FS_NODE_P_IDX_ROOT) {
            puts("cd: Trainer-chan!! Ngga bisa balik dari root!\n");
            exit();
        }
        while (i < 64) {
            if (*current_dir == i) {
                *current_dir = node_fs_buffer.nodes[i].parent_node_index;
                exit();
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
                puts("cd: Trainer-chan!! Direktori tidak ditemukan!\n");
                strclr(temp);
                strclr(path);
                exit();
            }
            strclr(temp);
            j = 0;
            k = 0;
            i++;
        }
        if (i == 0) {
            puts("cd: Trainer-chan!! Argumennya invalid!\n");
            strclr(temp);
            strclr(path);
            exit();
        }
        msg.current_directory = tempdst;
        setMessage(&msg);
    }

    exit();
}
