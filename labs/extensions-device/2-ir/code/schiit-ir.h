#ifndef __SCHIIT_IR_H__
#define __SCHIIT_IR_H__

// example of defining a controller.  this is for the slim 
// aluminum schiit controler.
#include "dev-tsop4838.h"

enum keys {
    GT_O = 0xef11bf1,
    UP = 0xdf21bf1,
    DOWN = 0xcf31bf1,
    O_GT = 0xbf41bf1,
    MUTE = 0xaf51bf1,
};

static ir_keys_t keys[] = {
    key_press_mk(NOISE, NOISE),
    key_press_mk(>0,    GT_O),
    key_press_mk(UP,    UP),
    key_press_mk(DOWN,  DOWN),
    key_press_mk(0>,    O_GT),
    key_press_mk(x,     MUTE),
    key_press_mk(0,     0)          // last entry must have a NULL key.
};

#endif
