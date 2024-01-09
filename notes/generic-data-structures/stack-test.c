#include "libunix.h"
#include "stack-T.h"
    
// defines u32_stack_T and the varioius helpers.
gen_stack_T(u32_stack, uint32_t, 1024)

// define a second one with different names.
gen_stack_T(stack_u64, uint64_t, 20)

int main(void) {
    output("about to test\n");
    
    // declare both stack types
    STACK_T(u32_stack) s = u32_stack_mk();
    STACK_T(stack_u64) s32 = stack_u64_mk();

    // put the same integers in both.
    for(unsigned i = 0; i < 10; i++)  {
        u32_stack_push(&s, i);
        stack_u64_push(&s32, i);
    }

    // easy clone: make two copies and check that 
    // removing gives the same results.
    STACK_T(u32_stack) s1 = s;
    STACK_T(u32_stack) s2 = s;

    // make sure give same value.
    while(u32_stack_cnt(&s1)) {
        uint32_t x1 = u32_stack_pop(&s1);
        uint32_t x2 = u32_stack_pop(&s2);
        output("x1=%d,x2=%d\n", x1,x2);
        assert(x1==x2);
    }

    // now compare the new implementations.  should be the
    // same since stored small ints in them.
    for(unsigned i = 0; i < 10; i++) {
        uint32_t x = u32_stack_pop(&s);
        output("x=%d\n", x);

        uint32_t x2 = stack_u64_pop(&s32);
        assert(x2 == x);
    }

    return 0;
}
