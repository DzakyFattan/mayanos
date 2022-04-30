#include "header/program.h"

void exec(struct file_metadata *metadata, int segment) {
    interrupt(0x21, 0xF, metadata, segment, 0);
}

void exit() {
    struct file_metadata meta;
    struct message msg;
    char tmp[9];
    int current_segment, i;
    
    current_segment = getCurrentSegment();
    getMessage(&msg, current_segment);
    meta.node_name = msg.next_command;

    if (meta.node_name[0] == '.' && meta.node_name[1] == '/') {
        // local directory
        meta.parent_index = msg.current_directory;
        for (i = 0; i < 9; i++) tmp[i] = '\0';
        strcpy(tmp, meta.node_name);
        meta.node_name = tmp+2;
    } else {
        meta.parent_index = 0x00;
    }

    exec(&meta, msg.next_program_segment);
}
