#include "rpi.h"
#include "pinned-vm.h"
#include "vector-base.h"

#include "mmu.h"


// kernel domain = 1.  we leave 0 free so can sanity check.
enum { kern_dom = 1 };

enum { MB = 1024 * 1024 };

// physical address of our kernel: used to map.
typedef struct {
    uint32_t addr, nbytes;
    enum { MEM_DEVICE, MEM_RW, MEM_RO } type;
} pr_ent_t;
static inline pr_ent_t
pr_ent_mk(uint32_t addr, uint32_t nbytes, int type) {
    demand(nbytes == 1024*1024, only doing sections first);
    return (pr_ent_t) { 
        .addr = addr, 
        .nbytes = nbytes,
        .type = type
    };
}

typedef struct {
#   define MAX_ENT 8
    unsigned n;
    pr_ent_t map[MAX_ENT];
} prmap_t;

static inline void prmap_push(prmap_t *p, pr_ent_t e) {
    assert(p->n < MAX_ENT);
    p->map[p->n++] = e;
}


// memory map of address space.  each chunk of memory should have an entry.
// must be aligned to the size of the memory region.
//
// basic map of our address space: should add sanity checking that ranges
// are not overlapping.   
//
// kinda janky since still has assumptions encoded.
//
// in real life would want more control over region attributes.
//
// we don't seem able to search in the TLB if MMU is off.  
// 
static prmap_t prmap_default_mk(void) {
    prmap_t p = {};
    prmap_push(&p, pr_ent_mk(0x20000000, MB, MEM_DEVICE));
    prmap_push(&p, pr_ent_mk(0x20100000, MB, MEM_DEVICE));
    prmap_push(&p, pr_ent_mk(0x20200000, MB, MEM_DEVICE));

    // program currently should fit on 1 page.
    prmap_push(&p, pr_ent_mk(0x00000000, MB, MEM_RW));
    // heap
    prmap_push(&p, pr_ent_mk(0x00100000, MB, MEM_RW));

    prmap_push(&p, pr_ent_mk(INT_STACK_ADDR-MB, MB, MEM_RW));
    prmap_push(&p, pr_ent_mk(STACK_ADDR-MB, MB, MEM_RW));

    return p;
}

static prmap_t prmap_pin(prmap_t *p) {
    // currently everything is uncached.
    pin_t g = pin_mk_global(kern_dom, perm_rw_priv, MEM_uncached);

    for(unsigned i = 0; i < p->n; i++) {
        pr_ent_t *e = &p->map[i];
        assert(e->nbytes == MB);
        switch(e->type) {
        case MEM_DEVICE:
                pin_ident_map_device(i, e->addr, kern_dom);
                break;
        case MEM_RW:
                pin_ident_map(i, e->addr, g);
                break;
        case MEM_RO: panic("not handling\n");
        default: panic("unknown type: %d\n", e->type);
        }
    }
}

static void check_pinned(uint32_t va) {
    uint32_t r;
    if(tlb_contains_va(&r, va)) {
        output("success: TLB contains %x, returned %x\n", va, r);
        assert(va == r);
    } else
        panic("TLB should have %x: returned %x [reason=%b]\n", 
            va, r, bits_get(r,1,6));
}

void notmain(void) { 
    assert(!mmu_is_enabled());

    // we have to clear the MMU before setting any entries.
    staff_mmu_init();

    // these are rw, uncached device memory.
    pin_ident_map_device(0, 0x20000000, kern_dom);
    pin_ident_map_device(1, 0x20100000, kern_dom);
    pin_ident_map_device(2, 0x20200000, kern_dom);

    // default: make everything uncached.
    pin_t g = pin_mk_global(kern_dom, perm_rw_priv, MEM_uncached);

    // map the first MB: shouldn't need more memory than this.
    pin_ident_map(3, 0x00000000, g);

    // map the heap: for lab cksums must be at 0x100000.
    kmalloc_init_set_start(MB, MB);
    pin_ident_map(4, 0x00100000, g);

    // map the kernel stack
    pin_ident_map(5, STACK_ADDR-MB, g);

    // if we don't setup the interrupt stack = super bad infinite loop
    pin_ident_map(6, INT_STACK_ADDR-MB, g);

    // we make a simplified MMU setup that 
    //  1. switches a single time into MMU enabled (so don't have to worry about
    //     back and forth)
    //  2. has a single ASID.
    // 
    // we do a one time setup: you need the cp15_set_procid_ttbr0 from your
    // cs140e lab and merge into a new routine that 
    // 1. initializes everything.
    // asid to 1.   

    // 0 filled page table to get fault on any lookup.
    void *null_pt = kmalloc(4096 * 4);
    demand((unsigned)null_pt % (1<<14) == 0, must be 14 bit aligned);

#if 0
    // wait: this should invalidate?
    // either invalidation is wrong, or we have to clear every pinned entry.
    cp15_dtlb_inv();
    cp15_itlb_inv();
    cp15_tlbs_inv();
#endif

    // right now we just have a single domain?
    staff_domain_access_ctrl_set(DOM_client << kern_dom*2);

    // install the default vectors.
    extern uint32_t default_vec_ints[];
    vector_base_set(default_vec_ints);

    output("about to turn on mmu\n");
    staff_mmu_on_first_time(1, null_pt);
    assert(mmu_is_enabled());

    output("enabled!\n");

    output("going to check entries are pinned\n");
    check_pinned(0);
    check_pinned(MB);
    check_pinned(0x20000000);
    check_pinned(0x20100000);
    check_pinned(0x20200000);

    void *bad_addr = (void*)(0x3 * MB);
    output("about to reference unmapped section address: %x\n", bad_addr);
    put32(bad_addr, 4);
    not_reached();

    // unimplemented();
}
