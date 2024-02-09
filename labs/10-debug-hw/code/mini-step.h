// very simple version of single stepping.
#ifndef __MINI_WATCH_H__
#define __MINI_WATCH_H__
#include "full-except.h"

typedef struct {
    uint32_t    fault_pc;      // pc faulted at.
    regs_t      *regs;           // full set of registers.
} step_fault_t;

static inline step_fault_t 
step_fault_mk(
    uint32_t fault_pc,
    regs_t *regs) 
{
    return (step_fault_t) { 
        .fault_pc = fault_pc, 
        .regs = regs
    };
}

// same, except <addr> is the faulting location.
typedef void (*step_handler_t)(void *data, step_fault_t *fault);

// one time initialize.  setup to call <h> with <data> and the fault.
void mini_step_init(step_handler_t h, void *data);

// run <fn> in single step mode with <arg>
uint32_t mini_step_run(void (*fn)(void*), void *arg);
#endif
