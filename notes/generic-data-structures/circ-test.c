#include "libunix.h"
#include "circular-T.h"
    
// defines u32_cq_T
gen_circular(u32, uint32_t, 1024)

int main(void) {
    output("about to test\n");
    
    CQ_T(u32) cq = u32_cq_mk(1);

    for(unsigned i = 0; i < 10; i++) 
        u32_cq_push(&cq, i);

    // easy clone.
    CQ_T(u32) cq1 = cq;
    CQ_T(u32) cq2 = cq;

    // make sure give same value.
    uint32_t x1;
    while(u32_cq_pop_nonblk(&cq1, &x1)) {
        uint32_t x2 = u32_cq_pop(&cq2);
        output("x1=%d,x2=%d\n", x1,x2);
        assert(x1==x2);
    }

    for(unsigned i = 0; i < 10; i++) {
        uint32_t x = u32_cq_pop(&cq);
        output("x=%d\n", x);
    }

    return 0;
}
