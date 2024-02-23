// complete working system call example using the <full-except.c> code.
#include "rpi.h"
#include "full-except.h"

// simple +1 system call
static int syscall_handler(regs_t *r) {
    uint32_t spsr = r->regs[REGS_CPSR];

    // i think the sp and spsr should be the same for everyone
    // if we clear the carry flags.
    trace("hi from handler: r0=%d, pc=[%x], sp=%x, spsr=%x\n", 
        r->regs[0], 
        r->regs[REGS_PC], 
        r->regs[REGS_SP], 
        cpsr_clear_carry(spsr));

    if(mode_get(spsr) != USER_MODE) 
        panic("we came from mode %s, expected USER_MODE\n", mode_str(spsr));

    return r->regs[0] + 1;
}

// assembly that does a swi instruction (see full-except-asm.S)
int syscall_trampoline(int sysnum, ...);

// is_align8
#include "pointer-T.h"

regs_t start_regs;

void syscall_test(void) {
    int i,r;
    for(r = i = 0; i < 10; i++) {
        r = syscall_trampoline(r);
        trace("iter=%d: result=%d\n", i, r);
        assert((i+1)==r);
    }

    trace("done! %d iterations: switching back\n", i);

    // resume: not pc will point one after the swi instruction (you 
    // can look in the .list file
    switchto(&start_regs);
}

// allocate an 8byte aligned stack and return a pointer
// to the end
uint32_t stack_alloc(unsigned stack_size) {
    demand(stack_size>1024, too small);
    demand(stack_size%8 == 0, not aligned);

    void *stack = kmalloc_aligned(stack_size, 8);

    uint32_t sp = (uint32_t)stack + stack_size;
    demand(sp % 8 == 0, sp is not aligned);
    return sp;
}

void notmain(void) {
    // setup the handlers.
    full_except_install(0);
    full_except_set_syscall(syscall_handler);

    // so all allocations start in the same place. (put in cstart?)
    kmalloc_init();     

    // get our current cpsr and clear the carry and set the mode
    uint32_t cpsr = cpsr_inherit(USER_MODE, cpsr_get());

    // XXX: which code had the partial save?  the ss rwset?
    regs_t r = (regs_t) {
        .regs[REGS_PC] = (uint32_t)syscall_test,      // where we want to jump to
        .regs[REGS_SP] = stack_alloc(8192*4),      // the stack pointer to use.
        .regs[REGS_LR] = (uint32_t)clean_reboot, // where to jump if return.
        .regs[REGS_CPSR] = cpsr             // the cpsr to use.
    };
    
    switchto_cswitch(&start_regs, &r);
    trace("done!\n");
}
