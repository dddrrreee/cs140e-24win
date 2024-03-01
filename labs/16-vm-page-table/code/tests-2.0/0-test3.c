#include "rpi.h"
#include "pinned-vm.h"

void notmain(void) { 
    // map the heap: for lab cksums must be at 0x100000.
    kmalloc_init_set_start(MB, MB);

    // turn on the pinned MMU: identity map.
    procmap_t p = procmap_default_mk(kern_dom);
    pin_mmu_on(&p);

    // if we got here MMU must be working b/c we have accessed GPIO/UART
    output("hello: mmu must be on\n");

    // unmapped memory.
    void *bad_addr = (void*)(0x3 * MB);
    output("about to reference unmapped section address: %x\n", bad_addr);

    put32(bad_addr, 4);
    not_reached();

    // unimplemented();
}
