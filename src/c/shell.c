#include "header/color.h"
#include "header/filesystem.h"
#include "header/message.h"
#include "header/std_type.h"
#include "header/string.h"
#include "header/textio.h"

void bufferToMessage(char *input_buf, struct message *msg);

int main() {
    char input_buf[64];
    char path_str[128];
    char command[16];
    int i = 0;
    int j = 0;
    int scrollLine;
    struct message msg;
    struct file_metadata meta;

    byte current_dir = FS_NODE_P_IDX_ROOT;
    msg.current_directory = current_dir;
    msg.next_program_segment = 0x200;

    putsColor("Pin Pon!! This is MayanOS!! >//<\r\n", BROWN);
    putsColor(":::=======  :::====  ::: === :::====  :::= === :::====  :::=== \n", YELLOW);
    putsColor("::: === === :::  === ::: === :::  === :::===== :::  === :::    \n", YELLOW);
    putsColor("=== === === ========  =====  ======== ======== ===  ===  ===== \n", YELLOW);
    putsColor("===     === ===  ===   ===   ===  === === ==== ===  ===     ===\n", YELLOW);
    putsColor("===     === ===  ===   ===   ===  === ===  ===  ======  ====== \n", YELLOW);
    putsColor("\nMaya siap membantu Trainer-chan, You Copy?! ( ^ w ^)7\r\n", BROWN);

    while (true) {
        putsColor("MayanOS:", BROWN);
        interrupt(0x21, 0xC, path_str, current_dir, 0);
        puts("$ ");

        gets(input_buf);
        while (input_buf[i] != ' ' && input_buf[i] != '\0') {
            command[i] = input_buf[i];
            i++;
        }
        command[i] = '\0';
        i++;
        if (strcmp(command, "exit")) {
            puts("\nTrainer-chan mau pergi? :( Kapan-kapan main ke sini lagi ya! ( ^ w ^) <3\r\n");
        } else if (strcmp(command, "ls")) {
            puts("ls\n");
        } else if (strcmp(command, "cd")) {
            // bufferToMessage(input_buf, &msg);
            strcpy(msg.arg1, input_buf);
            setMessage(&msg);
            meta.node_name = "cd";
            meta.parent_index = 0x00; // bin
            exec(&meta, 0x6000);
        } else if (strcmp(command, "cp")) {
            bufferToMessage(input_buf, &msg);
            setMessage(&msg);
            meta.node_name = "cp";
            meta.parent_index = 0x00; // bin
            exec(&meta, 0x5000);
        } else if (strcmp(command, "mkdir")) {
            puts("mkdir\n");
        } else if (strcmp(command, "rm")) {
            puts("rm\n");
        } else if (strcmp(command, "cat")) {
            puts("cat\n");
        } else if (strcmp(command, "mv")) {
            puts("mc\n");
        } else if (strcmp(input_buf, "Aku sayang sama Maya-chin")) {
            putsColor("Hehe, Maya juga sayang sama Trainer-chan ( ^ w ^) <3<3<3\r\n", BROWN);
        } else {
            putsColor("Maya ngga ngerti perintah Trainer-chan (# -_-)\r\n", BROWN);
        }
        strclr(input_buf);
        strclr(path_str);
        strclr(command);
        i = 0;
        j = 0;
    }
}

void bufferToMessage(char *input_buf, struct message *msg) {
    int i, j;

    // clear message
    msg->current_directory = 0;
    msg->next_program_segment = 0;
    for (i = 0; i < 64; i++) {
        msg->arg1[i] = 0;
        msg->arg2[i] = 0;
        msg->arg3[i] = 0;
        msg->arg4[i] = 0;
    }

    i = 0;
    while (input_buf[i] != '\0' && input_buf[i] != ' ') {
        i++;
    };
    if (input_buf[i] == '\0')
        return; // tidak ada argumen

    // argumen pertama
    i++;
    j = 0;
    while (input_buf[i] != '\0' && input_buf[i] != ' ') {
        msg->arg1[j] = input_buf[i];
        i++;
        j++;
    }

    // argumen kedua
    if (input_buf[i] == '\0')
        return; // tidak ada argumen
    i++;
    j = 0;
    while (input_buf[i] != '\0' && input_buf[i] != ' ') {
        msg->arg2[j] = input_buf[i];
        i++;
        j++;
    }

    // argumen tiga
    if (input_buf[i] == '\0')
        return; // tidak ada argumen
    i++;
    j = 0;
    while (input_buf[i] != '\0' && input_buf[i] != ' ') {
        msg->arg3[j] = input_buf[i];
        i++;
        j++;
    }

    // argumen empat
    if (input_buf[i] == '\0')
        return; // tidak ada argumen
    i++;
    j = 0;
    while (input_buf[i] != '\0' && input_buf[i] != ' ') {
        msg->arg3[j] = input_buf[i];
        i++;
        j++;
    }
}