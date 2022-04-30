#include "header/message.h"

void setMessage(struct message *msg, int segment) {
    int sector = ((segment - 0x2000) / 0x1000) + 0x105;

    writeSector(msg, sector);
}

void getMessage(struct message *msg, int segment) {
    int sector;

    sector = ((segment - 0x2000) / 0x1000) + 0x105;

    readSector(msg, sector);
}