#include "rpi.h"
#include "pinned-vm.h"

void notmain(void) { 
    output("hello\n");

    enum { MB = 1024 * 1024 };

    for(uint32_t va = 0; va < 128 * MB; va += MB) {
        uint32_t r;
        if(tlb_contains_va(&r, va)) {
            output("reason = %b\n", bits_get(r, 1, 6));
            panic("TLB should be empty!  %x returned %b\n", va, r);
        }
    }
}
