#include "header/program.h"

void exec(struct file_metadata *metadata, int segment) {
    interrupt(0x21, 0xF, metadata, segment, 0);
}

void exit() {
    struct file_metadata meta;
    struct message msg;
    int current_segment;
    
    current_segment = getCurrentSegment();
    getMessage(&msg, current_segment);
    meta.node_name = msg.next_command;
    meta.parent_index = 0x0;
    exec(&meta, msg.next_program_segment);
}
