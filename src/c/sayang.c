#include "header/message.h"
#include "header/program.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/utils.h"
#include "header/string.h"
#include "header/color.h"

int main() {
    char *input;
    struct message msg;
    struct message msg_next;
    int i, j;

    getMessage(&msg, getCurrentSegment());
    getMessage(&msg_next, msg.next_program_segment);
    msg_next.current_directory = msg.current_directory;
    setMessage(&msg_next, msg.next_program_segment);

    if (strcmp(msg.arg1, "maya-chin")) {
        if (strcmp(msg.arg3, "kali")) {
            if (msg.arg2[0] > '0' && msg.arg2[0] < ':') {
                j = msg.arg2[0] - '0';
                for (i = 0; i < j; i++) {
                    putsColor("Maya juga sayang sama Trainer-chan ( ^ w ^) <3<3<3\n\n", BROWN);
                } 
            }
        }
    }

    exit();
}