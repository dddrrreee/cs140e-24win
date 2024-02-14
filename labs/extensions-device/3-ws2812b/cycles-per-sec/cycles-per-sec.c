// low level print that makes a bit easier to debug.
#include "rpi.h"
#include "cycle-count.h"

enum { NTRIALS = 5 };

void measure(unsigned trial);

void notmain(void) {
    for(int i = 0; i < NTRIALS; i++) 
        measure(i);
}

// put after so gcc doesn't inline
void measure(unsigned trial) {
    printk("trial %d:\n", trial);

#   define One_M  (1000UL*1000UL)
#   define One_B  (1000UL*One_M)

    
    // crudely compute the number of cycles for 1sec.
    unsigned cyc_start = cycle_cnt_read();
    unsigned s = timer_get_usec();

    // there are 1000*1000 (1M) useconds in a second
#   define US_PER_SEC One_M
    while((timer_get_usec() - s) < US_PER_SEC)
        ;
    unsigned cyc_per_sec = cycle_cnt_read() - cyc_start;

    // div_rem_t d = div_uint32(cyc_per_sec, 1000*1000);
    printk("cycles per sec = %u (%u.%u million)\n", 
            cyc_per_sec,
            cyc_per_sec/One_M, 
            cyc_per_sec%One_M);


    // cheat by using libgcc.a to do division: see Makefile
    uint32_t ns_per_sec = One_B;

    // there are 1000*1000*1000 (1B) nanosec in a second
#   define NS_PER_SEC One_B
    printk("ns per cyc = %u.%u nanoseconds\n", 
            NS_PER_SEC / cyc_per_sec,
            NS_PER_SEC % cyc_per_sec);
}

