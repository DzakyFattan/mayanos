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
    getMessage(&msg, getCurrentSegment());
    input = msg.arg1;

    puts(input);
    puts("\n");
    exit();
}