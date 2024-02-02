#ifndef __WAIT_ROUTINES_H__
#define __WAIT_ROUTINES_H__
#include "cycle-count.h"

// delay for <ncycles> measured from the cycle <start_cycle>.  
// until that point, repeatedly calls <fn> if <fn> is not equal
// to null.
//
// returns the cycle counter reading that caused it to exit its
// delay loop.
//
// key: the use of <start_cycle> makes it possible to have 
// multiple delays without cumulative error.
static inline uint32_t 
wait_ncycles_exact_fn(
    uint32_t start_cycle,   // cycle we start counting from.
    uint32_t ncycles,       // stop when start_cycle_ncycle have passed
    void (*fn)(void))       // call while spinning if not null.
{
    todo("delay using cycle_cnt_read(): call fn if not null");
}

// same but no fn().
static inline uint32_t 
wait_ncycles_exact(uint32_t s, uint32_t n) {
    return wait_ncycles_exact_fn(s,n,0);
}

// delay for <n> cycles: if <fn> is non-null, call it 
// when waiting.
static inline uint32_t 
wait_ncycles_fn(uint32_t ncycles, void (*fn)(void)) {
    uint32_t s = cycle_cnt_read();
    return wait_ncycles_exact_fn(s, ncycles, fn);
}

// same, but no yield.
static inline uint32_t wait_ncycles(uint32_t ncycles) {
    return wait_ncycles_fn(ncycles, 0);
}

#endif
