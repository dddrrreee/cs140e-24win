#include "rpi.h"

void notmain(void) {
    printk("PI: hello world\n");
    clean_reboot();
}
