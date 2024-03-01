// a test to create two different address spaces with non-identity
// maps for one page and make sure that loads and stores resolve
// correctly.
#include "rpi.h"
#include "pt-vm.h"

static void data_fault(regs_t *r) {
    panic("we got a data abort fault at pc=%x\n", r->regs[15]);
}
static void prefetch_fault(regs_t *r) {
    panic("we got a prefetch abort fault at pc=%x\n", r->regs[15]);
}

void notmain(void) { 
    enum { OneMB = 1024*1024};
    // map the heap: for lab cksums must be at 0x100000.
    kmalloc_init_set_start((void*)MB, MB);
    assert(!mmu_is_enabled());

    // b4-42
    // staff_domain_access_ctrl_set(~0);   // DOM_client << DOM);
    enum { dom_kern = 1,            // domain for kernel=1
           dom_user = 2 };          // domain for user = 2
    uint32_t d = (DOM_client << dom_kern*2)
                |(DOM_client << dom_user*2);

    vm_pt_t *pt = vm_pt_alloc(PT_LEVEL1_N);
    vm_mmu_init(~0);

    // just like last lab.  setup a data abort handler.
    full_except_install(0);
    full_except_set_data_abort(data_fault);
    full_except_set_prefetch(prefetch_fault);


    // see 3-151 for table, or B4-9
    //    APX = 0, AP = 1
    //    APX << 2 | AP
    uint32_t no_user = perm_rw_priv;

    // description of device memory
    pin_t dev  = pin_mk_global(dom_kern, no_user, MEM_device);
    // description of kernel memory
    pin_t kern = pin_mk_global(dom_kern, no_user, MEM_uncached);

    // all the device memory: identity map
    vm_map_sec(pt, 0x20000000, 0x20000000, dev);
    vm_map_sec(pt, 0x20100000, 0x20100000, dev);
    vm_map_sec(pt, 0x20200000, 0x20200000, dev);

    // map first two MB for the kernel
    vm_map_sec(pt, 0, 0, kern);
    vm_map_sec(pt, OneMB, OneMB, kern);

    // kernel stack
    vm_map_sec(pt, STACK_ADDR-OneMB, STACK_ADDR-OneMB, kern);

    // our interrupt stack.
    vm_map_sec(pt, INT_STACK_ADDR-OneMB, INT_STACK_ADDR-OneMB, kern);

#if 0
    // actually: we don't do 16mb r/n.  should extend it.
    enum { ASID1 = 1, ASID2 = 2 };
    pin_t user1 = pin_16mb(pin_mk_user(dom_kern, ASID1, no_user, MEM_uncached));
    pin_t user2 = pin_16mb(pin_mk_user(dom_kern, ASID2, no_user, MEM_uncached));
    
    uint32_t user_addr = OneMB * 16;
    assert((user_addr>>12) % 16 == 0);
    uint32_t phys_addr1 = user_addr;
    uint32_t phys_addr2 = user_addr+16*OneMB;
    
    PUT32(phys_addr1, 0xdeadbeef);
    PUT32(phys_addr2, 0xdeadbeef);

    uint32_t user_idx = idx;
    pin_mmu_sec(idx++, user_addr, phys_addr1, user1);
    assert(idx<8);
#endif

    // actually: we don't do 16mb r/n
    enum { ASID1 = 1, ASID2 = 2 };
    pin_t user1 = pin_mk_user(dom_kern, ASID1, no_user, MEM_uncached);
    pin_t user2 = pin_mk_user(dom_kern, ASID2, no_user, MEM_uncached);
    assert(!user1.G);
    assert(!user2.G);
    
    uint32_t user_addr = OneMB*2;
    uint32_t phys_addr1 = user_addr;
    uint32_t phys_addr2 = user_addr+16*OneMB;

    uint32_t val1 = 1;
    uint32_t val2 = 2;
    
    PUT32(phys_addr1, val1);
    PUT32(phys_addr2, val2);

    vm_pt_t *pt1 = vm_dup(pt);
    vm_map_sec(pt1, user_addr, phys_addr1, user1);
    assert(vm_lookup(pt1, user_addr));
    uint32_t pa;


    if(!vm_xlate(&pa, pt1, user_addr))
        panic("can't translate %x\n", user_addr);
    else
        output("mapped %x->%x\n", user_addr, pa);

    if(pa != user_addr)
        panic("translated va=%x --> pa=%x\n", user_addr,pa);

    vm_pt_t *pt2 = vm_dup(pt);
    vm_map_sec(pt2, user_addr, phys_addr2, user2);

    trace("about to enable\n");

    vm_mmu_switch(pt1,0x140e,ASID1);
    vm_mmu_enable();

    assert(mmu_is_enabled());
    trace("MMU is on and working!\n");

    output("about to GET32\n");
    uint32_t x = GET32(user_addr);
    trace("asid 1 = got: %x\n", x);
    assert(x == val1);
    PUT32(user_addr, x+1);

    vm_mmu_disable();
    assert(!mmu_is_enabled());
    trace("MMU is off!\n");
    trace("phys addr1=%x\n", GET32(phys_addr1));
    assert(GET32(phys_addr1) == val1+1);

    vm_mmu_switch(pt2,0x140e + 1,ASID2);
    vm_mmu_enable();

    x = GET32(user_addr);
    trace("asid 2 = got: %x\n", x);
    assert(x == val2);
    PUT32(user_addr, x+1);

    vm_mmu_disable();
    trace("checking user2=%x\n", GET32(phys_addr2));
    assert(GET32(phys_addr2) == val2+1);

    trace("SUCCESS!\n");
}
