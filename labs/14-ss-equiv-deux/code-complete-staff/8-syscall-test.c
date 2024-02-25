// complete working system call example using the <full-except.c> code.
#include "rpi.h"
#include "full-except.h"

enum { SYS_PLUS1, SYS_DONE };       // our two system calls.

// switch back to after test.
static regs_t start_regs;           

// used to saved sp is legal.
static uint32_t stack_start, stack_end;

// simple system call to test.  either increment the first argument,
// or switch back to the code that starts the test.
//
// note: this will work with the INT_STACK as a stack pointer.
static int syscall_handler(regs_t *r) {
    uint32_t spsr = r->regs[REGS_CPSR];
    uint32_t sp = r->regs[REGS_SP];

    // i think the sp and spsr should be the same for everyone
    // if we clear the carry flags.
    trace("hi from handler: r0=%d, pc=[%x], sp=[%x], spsr=%x\n", 
        r->regs[0], 
        r->regs[REGS_PC], 
        sp,
        cpsr_clear_carry(spsr));

    // check the stack pointer is where it should be.
    if(sp < stack_start || sp > stack_end)
        panic("illegal stack pointer sp=%x, should be within [%x,%x)\n",
            sp, stack_start, stack_end);

    // should always be coming from user mode.
    if(mode_get(spsr) != USER_MODE) 
        panic("we came from mode %s, expected USER_MODE\n", mode_str(spsr));

    uint32_t sysno = r->regs[0];
    switch(sysno) {
    case SYS_PLUS1:
        return r->regs[1] + 1;
    case SYS_DONE:
        // resume: note pc will point one after the swi instruction (you 
        // can look in the .list file
        switchto(&start_regs);
        not_reached();
    default: panic("illegal syscall: %d\n", sysno);
    }
}

// assembly that does a swi instruction (see full-except-asm.S)
int syscall_trampoline(int sysnum, ...);

// this runs at user level.  calls the system call to increment
// a counter, then switches back when done.
void syscall_test(void) {
    int i,r;
    for(r = i = 0; i < 10; i++) {
        if(!mode_is_user())
            panic("should be at user level!\n");

        r = syscall_trampoline(SYS_PLUS1, r);
        trace("iter=%d: result=%d\n", i, r);
        assert((i+1)==r);
    }

    trace("done! %d iterations: switching back\n", i);

    // switch back to the original caller.
    syscall_trampoline(SYS_DONE);
    panic("should not return\n");
}

void notmain(void) {
    // setup the handlers.
    full_except_install(0);
    full_except_set_syscall(syscall_handler);

    // so all allocations start in the same place. (put in cstart?)
    kmalloc_init();     

    if(!mode_is_super())
        panic("our mode is not set correctly?\n");

    uint32_t cpsr = cpsr_get();
    uint32_t mode_orig = mode_get(cpsr);

    // get our current cpsr and clear the carry and set the mode
    cpsr = cpsr_inherit(USER_MODE, cpsr);

    // allocate stack, record start and end for bounds checking.
    uint32_t stack_size = 8192*4;
    stack_start = (uint32_t)kmalloc_aligned(stack_size, 8);
    stack_end = stack_start + stack_size;
    demand(stack_end % 8 == 0, sp is not aligned);

    // setup initial thread registers.
    regs_t r = (regs_t) {
        .regs[REGS_PC] = (uint32_t)syscall_test,    // where we want to jump to
        .regs[REGS_SP] = stack_end,                 // the stack pointer to use.
        .regs[REGS_LR] = (uint32_t)clean_reboot,    // where to jump if return.
        .regs[REGS_CPSR] = cpsr                     // the cpsr to use.
    };
    
    // switch to user level: syscall handler will switch back when done.
    switchto_cswitch(&start_regs, &r);

    // check that mode is back the way it should be.
    if(!mode_is_super())
        panic("our mode is not SUPER: %b\n", mode_get(cpsr_get()));

    trace("SUCCESS!!\n");
}
