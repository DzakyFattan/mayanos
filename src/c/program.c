#include "header/program.h"

void exec(struct file_metadata *metadata, int segment) {
    interrupt(0x21, 0xF, metadata, segment, 0);
}

void exit() {
    interrupt(0x21, 0xE, 0x0, 0x0, 0x0);
}

void setMessage(struct message *msg) {
    interrupt(0x21, 0x3, msg, MSG_SECTOR, 0x0, 0x0);
}

void getMessage(struct message *msg) {
    interrupt(0x21, 0x2, msg, MSG_SECTOR, 0x0, 0x0);
}