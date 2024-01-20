#include "rpi.h"

void todo_test(char *msg) {
    output("todo_test=<%s>\n", msg);
    clean_reboot();
}

void todo_test_asm(void);

void notmain(void) {
    output("calling asm\n");
    todo_test_asm();
}

