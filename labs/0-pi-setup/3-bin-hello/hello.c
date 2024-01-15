// <rpi.h> has definitions for <putk> and <clean_reboot>.
// <libpi.a> has their implementations.
#include "rpi.h"

void notmain(void) {
    putk("hello world from the pi\n");
    clean_reboot();
}
