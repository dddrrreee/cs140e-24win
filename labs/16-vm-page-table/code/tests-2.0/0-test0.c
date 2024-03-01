#include "rpi.h"
#include "pinned-vm.h"

#include "mmu.h"

void notmain(void) { 

    enum { OneMB = 1024 * 1024 };


    // not sure we can do this with the MMU off.

#if 0
    uint32_t r, va = 1024 * 1024;
    // this does contain.  and it says that the thing 
    if(tlb_contains_va(&r, va)) {
        output("reason = %b\n", bits_get(r, 1, 6));
        panic("TLB should be empty!  %x returned %x\n", va, r);
    }
#endif

    // kernel domain = 1.  we leave 0 free so can sanity check.
    enum { kern_dom = 1 };

    staff_mmu_init();
    assert(!mmu_is_enabled());
    staff_domain_access_ctrl_set(0b01 << kern_dom*2);


    // these are rw, uncached device memory.
    pin_ident_map_device(0, 0x20000000, kern_dom);
    pin_ident_map_device(1, 0x20100000, kern_dom);
    pin_ident_map_device(2, 0x20200000, kern_dom);

    // default: make everything uncached.
    pin_t g = pin_mk_global(kern_dom, perm_rw_priv, MEM_uncached);

    // map the first MB: shouldn't need more memory than this.
    pin_ident_map(3, 0x00000000, g);
    // map the heap: for lab cksums must be at 0x100000.
    pin_ident_map(4, 0x00100000, g);

    // map the kernel stack
    pin_ident_map(5, STACK_ADDR-OneMB, g);

    // if we don't setup the interrupt stack = super bad infinite loop
    pin_ident_map(6, INT_STACK_ADDR-OneMB, g);

    // we make a simplified MMU setup that 
    //  1. switches a single time into MMU enabled (so don't have to worry about
    //     back and forth)
    //  2. has a single ASID.
    // 
    // we do a one time setup: you need the cp15_set_procid_ttbr0 from your
    // cs140e lab and merge into a new routine that 
    // 1. initializes everything.
    // asid to 1.   

    output("about to turn on mmu\n");

    // this code uses the sequence on B2-25
    staff_set_procid_ttbr0(0x140e, kern_dom, (void*)0xfeedface);

    // note: this routine has to flush I/D cache and TLB, BTB, prefetch buffer.
    staff_mmu_enable();
    assert(mmu_is_enabled());

    output("enabled!\n");

}
