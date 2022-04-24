#include "header/std_type.h"
#include "header/string.h"
#include "header/textio.h"
#include "header/filesystem.h"
#include "header/color.h"

int main() {
    char input_buf[64];
    char path_str[128];
    char command[16];
    int i = 0;
    int j = 0;
    int scrollLine;

    byte current_dir = FS_NODE_P_IDX_ROOT;

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
            puts("cd\n");
        } else if (strcmp(command, "cp")) {
            puts("cp\n");
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