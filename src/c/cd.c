#include "header/fileio.h"
#include "header/filesystem.h"
#include "header/message.h"
#include "header/program.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/utils.h"
#include "header/string.h"

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
    struct message msg_next;
    struct node_filesystem node_fs_buffer;

    getMessage(&msg, getCurrentSegment());
    getMessage(&msg_next, msg.next_program_segment);
    current_dir = &(msg.current_directory);
    path_str = msg.arg1;

    readSector(&node_fs_buffer.nodes[0], FS_NODE_SECTOR_NUMBER);
    readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 0x1);

    strcpy(path, path_str);
    path_length = strlen(path);

    tempdst = *current_dir;
    if (strcmp(path, "..")) {
        if (*current_dir == FS_NODE_P_IDX_ROOT) {
            puts("cd: Trainer-chan!! Ngga bisa balik dari root!\n");
            exit();
        }
        while (i < 64) {
            if (*current_dir == i) {
                msg.current_directory = node_fs_buffer.nodes[i].parent_node_index;
                msg_next.current_directory = msg.current_directory;
                setMessage(&msg_next, msg.next_program_segment);
                exit();
            }
            i++;
        }
    } else if (strcmp(path, "/")) {
        msg.current_directory = FS_NODE_P_IDX_ROOT;
        msg_next.current_directory = msg.current_directory;
        setMessage(&msg_next, msg.next_program_segment);
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
        msg_next.current_directory = msg.current_directory;
        setMessage(&msg_next, msg.next_program_segment);
    }

    exit();
}
