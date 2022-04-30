#include "header/color.h"
#include "header/filesystem.h"
#include "header/message.h"
#include "header/program.h"
#include "header/std_type.h"
#include "header/string.h"
#include "header/textio.h"

void bufferToMessage(char *input_buf, struct message *msg);

void parseMultiProgram(char *input_buf, byte current_dir);

int main() {
    char input_buf[64];
    char path_str[128];
    char command[16];
    int i = 0;
    int j = 0;
    struct message msg;
    struct file_metadata meta;
    byte current_dir;

    while (true) {
        getMessage(&msg, 0x2000);
        current_dir = msg.current_directory;

        putsColor("MayanOS: ", BROWN);
        interrupt(0x21, 0xC, current_dir, 0, 0);
        puts("$ ");

        for (j = 0; j < 64; j++)
            input_buf[j] = '\0';
        gets(input_buf);
        parseMultiProgram(input_buf, current_dir);

        while (input_buf[i] != ' ' && input_buf[i] != '\0') {
            command[i] = input_buf[i];
            i++;
        }
        command[i] = '\0';
        i++;

        if (strcmp(command, "exit")) {
            puts("\nTrainer-chan mau pergi? :( Kapan-kapan main ke sini lagi ya! ( ^ w ^) <3\r\n");
        } else if (strcmp(command, "clear")) {
            interrupt(0x21, 0xA, 0x0, 0x0 ,0x0);
        } else if (strcmp(command, "ls")) {
            meta.node_name = "ls";
            meta.parent_index = 0x00;
            exec(&meta, 0x4000);
        } else if (strcmp(command, "cd")) {
            meta.node_name = "cd";
            meta.parent_index = 0x00; // bin
            exec(&meta, 0x4000);
        } else if (strcmp(command, "cp")) {
            meta.node_name = "cp";
            meta.parent_index = 0x00; // bin
            exec(&meta, 0x4000);
        } else if (strcmp(command, "mkdir")) {
            meta.node_name = "mkdir";
            meta.parent_index = 0x00; // bin
            exec(&meta, 0x4000);
        } else if (strcmp(command, "cat")) {
            meta.node_name = "cat";
            meta.parent_index = 0x00; // bin
            exec(&meta, 0x4000);
        } else if (strcmp(command, "mv")) {
            meta.node_name = "mv";
            meta.parent_index = 0x00; // bin
            exec(&meta, 0x4000);
        } else if (input_buf[0] == '.' && input_buf[1] == '/') {
            strcpy(meta.node_name, command + 2);
            meta.parent_index = current_dir;
            exec(&meta, 0x4000);
        } else if (strcmp(input_buf, "Aku sayang sama Maya-chin") || strcmp(input_buf, "sayang")) {
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
        msg->arg4[j] = input_buf[i];
        i++;
        j++;
    }
}

void parseMultiProgram(char *input_buf, byte current_dir) {
    int i = 0;
    int j;
    bool is_multi_program = false;
    struct message msg;
    char program[64];
    char command[9];
    int current_segment = 0x4000;

    // cek apakah multi program
    for (i = 0; i < 64; i++) {
        if (input_buf[i] == ';') {
            is_multi_program = true;
            break;
        }
    }

    if (!is_multi_program) {
        // bukan multi program, eksekusi seperti biasa
        // parse command
        bufferToMessage(input_buf, &msg);
        msg.current_directory = current_dir;
        msg.next_program_segment = 0x2000; // shell
        setMessage(&msg, current_segment);
        i = 0;
        while (input_buf[i] != ' ') {
            command[i] = input_buf[i];
            i++;
        }
    } else {
        i = 0;
        while (input_buf[i] != '\0') {
            // clear buffer
            for (j = 0; j < 64; j++)
                program[j] = '\0';

            j = 0;
            // copy command + arg
            while (input_buf[i] != ';' && input_buf[i] != '\0') {
                program[j] = input_buf[i];
                i++;
                j++;
            }
            i += 2;

            if (program[j] == ' ')
                program[j] = '\0';

            // copy next command
            for (j = 0; j < 9; j++)
                command[j] = '\0';
            j = 0;
            while (input_buf[i] != ' ' && input_buf[i] != '\0') {
                command[j] = input_buf[i];
                i++;
                j++;
            }
            i -= j;

            bufferToMessage(program, &msg);
            msg.current_directory = current_dir;
            msg.next_program_segment = current_segment + 0x1000;
            strcpy(msg.next_command, command);
            setMessage(&msg, current_segment);

            current_segment += 0x1000;
        }

        current_segment -= 0x1000;
        msg.next_program_segment = 0x2000;
        strcpy(msg.next_command, "shell");
        setMessage(&msg, current_segment);
    }
}