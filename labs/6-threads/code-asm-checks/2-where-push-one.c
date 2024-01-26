// Q: does push change sp before writing to it or after?
#include "rpi.h"

enum { val1 = 0xdeadbeef, val2 = 0xFAF0FAF0 };

// you write this in <asm-check.S>
//
// should take a few lines:
//  use the "push" instruction to push <val1>
//  onto the memory pointed to by <addr>.
//
// returns the final address
uint32_t *push_one(uint32_t *addr, uint32_t val1);

void notmain() {
    uint32_t v[4] = { 1, 2, 3, 4 };

    uint32_t *res = push_one(&v[2], val1);
    assert(res == &v[1]);

    // note this also shows you the order of writes.
    if(v[2] == val1) {
        assert(v[3] == 4);
        assert(v[1] == 2);
        assert(v[0] == 1);
        trace("wrote value before modifying pointer\n");
    } else if(v[1] == val1) {
        assert(v[3] == 4);
        assert(v[2] == 3);
        assert(v[0] == 1);
        trace("wrote value after modifying pointer\n");
    } else 
        panic("unexpected result\n");
}
