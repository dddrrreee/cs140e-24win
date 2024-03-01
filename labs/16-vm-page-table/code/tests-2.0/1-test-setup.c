// make a simple user mapping.
#include "rpi.h"
#include "pt-vm.h"

void notmain(void) { 
    assert(!mmu_is_enabled());

    enum { OneMB = 1024*1024};
    // map the heap: for lab cksums must be at 0x100000.
    kmalloc_init_set_start((void*)OneMB, OneMB);

    vm_pt_t *pt = vm_pt_alloc(PT_LEVEL1_N);

    vm_mmu_init(dom_all_access);

    enum { 
        dom_kern = 1, // domain id for kernel
        dom_user = 2  // domain id for user
    };          

    uint32_t no_user = perm_rw_priv;

    // device memory: kernel domain, no user access, 
    // memory is strongly ordered, not shared.
    pin_t dev  = pin_mk_global(dom_kern, no_user, MEM_device);
    // kernel memory: same, but is only uncached.
    pin_t kern = pin_mk_global(dom_kern, no_user, MEM_uncached);

    // index into the 8 pinned entries in tlb.
    unsigned idx = 0;
    // map all device memory to itself.  ("identity map")
    vm_map_sec(pt, 0x20000000, 0x20000000, dev);
    vm_map_sec(pt, 0x20100000, 0x20100000, dev);
    vm_map_sec(pt, 0x20200000, 0x20200000, dev); 

    // map first two MB for the kernel (it would be better to not
    // have 0 (null) mapped of course, but our code starts at 0x8000.
    vm_map_sec(pt, 0, 0, kern);                 
    vm_map_sec(pt, OneMB, OneMB, kern);     

    // map kernel stack (or nothing will work)
    vm_map_sec(pt, STACK_ADDR-OneMB, STACK_ADDR-OneMB, kern);

    // we don't take faults so don't need an exception stack.
    // vm_map_sec(pt, INT_STACK_ADDR-OneMB, INT_STACK_ADDR-OneMB, kern);

    // create a single user entry, also uncached.
    enum { ASID1 = 1 };

    // non-global entry with user permissions. user attribute: not global.  
    pin_t user1 = pin_mk_user(dom_kern, ASID1, no_user, MEM_uncached); // 6
    
    // read/write the first mb.
    uint32_t user_addr = OneMB * 16;
    assert((user_addr>>12) % 16 == 0);
    uint32_t phys_addr1 = user_addr;
    PUT32(phys_addr1, 0xdeadbeef);

    // user mapping: do an ident.
    vm_map_sec(pt, user_addr, phys_addr1, user1);
    assert(idx<8);

    trace("about to enable\n");

    // lockdown_print_entries("about to turn on first time");
    vm_mmu_switch(pt,0x140e,ASID1);

    vm_mmu_enable();


    // checking that we can access.
    uint32_t res;
    if(vm_lookup(pt,MB*10))
        panic("how contain?\n");
    else
        output("success?\n");

    assert(mmu_is_enabled());
    trace("MMU is on and working!\n");
    
    uint32_t x = GET32(user_addr);
    trace("asid 1 = got: %x\n", x);
    assert(x == 0xdeadbeef);
    PUT32(user_addr, 1);

    vm_mmu_disable();
    assert(!mmu_is_enabled());
    trace("MMU is off!\n");
    trace("phys addr1=%x\n", GET32(phys_addr1));
    assert(GET32(phys_addr1) == 1);

    trace("SUCCESS!\n");
}
