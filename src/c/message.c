#include "header/message.h"
#include "header/textio.h"

void setMessage(struct message *msg) {
    interrupt(0x21, 0x3, msg, MSG_SECTOR, 0x0);
}

void getMessage(struct message *msg) {
    interrupt(0x21, 0x2, msg, MSG_SECTOR, 0x0);
}