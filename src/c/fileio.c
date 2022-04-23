#include "header/fileio.h"

void write(struct file_metadata *metadata, enum fs_retcode *return_code) {
    struct node_filesystem node_fs_buffer;
    struct sector_filesystem sector_fs_buffer;
    struct map_filesystem map_fs_buffer;

    char *filename = metadata->node_name;
    byte parent = metadata->parent_index;
    byte node_index;
    unsigned int i, j, current_writing_byte;
    bool found = false, finished = false;
    byte parent_node_index;
    byte sector_needed, sector_available, empty_entry_index;
    struct sector_entry sector_entry_buf;


    readSector(map_fs_buffer.is_filled, FS_MAP_SECTOR_NUMBER);
    readSector(node_fs_buffer.nodes, FS_NODE_SECTOR_NUMBER);
    readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 0x1);
    readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);

    for (i = 0; i < 64 && !found; i++) {
        if (strcmp(node_fs_buffer.nodes[i].name, filename) && node_fs_buffer.nodes[i].parent_node_index == parent) {
            *return_code = FS_W_FILE_ALREADY_EXIST;
            return;
        }
    }

    found = false;
    for (i = 0; i < 64; i++) {
        if (strlen(node_fs_buffer.nodes[i].name) == 0) {
            node_index = i;
            found = true;
            break;
        }
    }
    if (!found) {
        *return_code = FS_W_MAXIMUM_NODE_ENTRY;
        return;
    }

    parent_node_index = metadata->parent_index;

    if (parent_node_index != 0xFF) {
        if (node_fs_buffer.nodes[parent_node_index].sector_entry_index != 0xFF) {
            *return_code = FS_W_INVALID_FOLDER;
            return;
        }
    }
 
    if (metadata->filesize == 0) {
        sector_needed = 0;
    } else {
        
        sector_needed = (metadata->filesize + 512 - 1) / 512;
    }

    sector_available = 0;
    for (i = 0; i < 512; i++) {
        if (!map_fs_buffer.is_filled[i]) {
            sector_available++;
        }
    }
    if ((sector_available < sector_needed) || (metadata->filesize > 8192)) {
        *return_code = FS_W_NOT_ENOUGH_STORAGE;
        return;
    }

    found = false;
    for (i = 0; i < 32; i++) {
        if (sector_fs_buffer.sector_list[i].sector_numbers[0] == 0) {
            empty_entry_index = i;
            found = true;
            break;
        }
    }
    if (!found && (metadata->filesize > 0)) {
        *return_code = FS_W_MAXIMUM_SECTOR_ENTRY;
        return;
    }
    strcpy(node_fs_buffer.nodes[node_index].name, metadata->node_name);
    node_fs_buffer.nodes[node_index].parent_node_index = metadata->parent_index;

    if (metadata->filesize == 0) {
        node_fs_buffer.nodes[node_index].sector_entry_index = FS_NODE_S_IDX_FOLDER;
    } else {
        node_fs_buffer.nodes[node_index].sector_entry_index = empty_entry_index; 
        j = 0;                                                                   
        finished = false;
        i = 0;
        current_writing_byte = 0;
        while (!finished && i <= 255) {
            if (!map_fs_buffer.is_filled[i]) {
                map_fs_buffer.is_filled[i] = true;

                sector_entry_buf.sector_numbers[j] = i;
                j++;

                writeSector(&metadata->buffer[current_writing_byte], i);

                current_writing_byte += 512;
                if (metadata->filesize <= current_writing_byte) {
                    finished = true;
                }
            }
            i++;
        }
        memcpy(sector_fs_buffer.sector_list[empty_entry_index].sector_numbers, sector_entry_buf.sector_numbers, 16);
    }
    writeSector(map_fs_buffer.is_filled, 0x100);
    writeSector(&node_fs_buffer.nodes[0], 0x101);
    writeSector(&node_fs_buffer.nodes[32], 0x102);
    writeSector(sector_fs_buffer.sector_list, 0x103);

    *return_code = FS_SUCCESS;
}

void read(struct file_metadata *metadata, enum fs_retcode *return_code) {
    struct node_filesystem node_fs_buffer;
    struct sector_filesystem sector_fs_buffer;
    char *filename = metadata->node_name;
    byte *test = 0x61;
    byte parent = metadata->parent_index;
    byte node_index;
    byte sector_entry_index;
    byte sector_nums_buffer[16];
    byte i;
    bool found = false;

    readSector(&node_fs_buffer.nodes[0], FS_NODE_SECTOR_NUMBER);
    readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 0x1);
    readSector(&sector_fs_buffer.sector_list[0], FS_SECTOR_SECTOR_NUMBER);

    for (i = 0; i < 64 && !found; i++) {
        if (strcmp(node_fs_buffer.nodes[i].name, filename) && node_fs_buffer.nodes[i].parent_node_index == parent) {
            node_index = i;
            found = true;
            break;
        }
    }

    if (!found) {
        *return_code = FS_R_NODE_NOT_FOUND;
        return;
    }
   
    sector_entry_index = node_fs_buffer.nodes[node_index].sector_entry_index;

    if (sector_entry_index == FS_NODE_S_IDX_FOLDER) {
        *return_code = FS_R_TYPE_IS_FOLDER;
        return;
    }

    memcpy(sector_nums_buffer, sector_fs_buffer.sector_list[sector_entry_index].sector_numbers, 16);
    metadata->filesize = 0;
    for (i = 0; i < 16; i++) {
        if (sector_nums_buffer[i] == 0) {
            break;
        }
        readSector(&(metadata->buffer[i * 512]), sector_nums_buffer[i]);
        metadata->filesize += 512;
    }

    *return_code = FS_SUCCESS;
}
