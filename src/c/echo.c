#include "header/fileio.h"
#include "header/filesystem.h"
#include "header/message.h"
#include "header/program.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/utils.h"
#include "string.h"

int main() {
    char *input;
    struct message msg;
    struct message msg_next;
    getMessage(&msg, getCurrentSegment());
    getMessage(&msg_next, msg.next_program_segment);
    msg_next.current_directory = msg.current_directory;
    setMessage(&msg_next, msg.next_program_segment);

    puts(msg.arg1);
    puts(" ");
    puts(msg.arg2);
    puts(" ");
    puts(msg.arg3);
    puts(" ");
    puts(msg.arg4);
    puts("\n");
    exit();
}