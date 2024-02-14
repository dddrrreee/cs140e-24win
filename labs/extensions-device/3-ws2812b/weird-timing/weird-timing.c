/*
 * engler: example of how alignment can make a difference in timing on the r/pi A+.  
 *
 * the underlying issue seems to be that the r/pi *appears* to have a 16 byte prefetch 
 * buffer, even with icache disabled.  whether four 4-byte instructions fit entirely 
 * within the buffer is entirely determined by alignment --- in this case, if the 
 * address of the first instruction is 16 byte aligned (addr % 16 == 0).
 *
 * if you run 4 instructions that entirely fit in the buffer (the first is 16 byte
 * aligned), then this will take less time than if the four instructions straddle the buffer.
 
 */
#include "rpi.h"
#include "cycle-count.h"

void check_align(void);

void notmain(void) {
    unsigned ntrials = 3;

    for(unsigned i = 0; i < ntrials; i++) {
        output("no icache: trial=%d: checking alignment\n");
        check_align();
    }

    caches_enable(); 
    for(unsigned i = 0; i < ntrials; i++) {
        output("icache on: trial=%d: checking alignment\n", i);
        check_align();
    }


    output("done!\n");
}

void check_align(void) { 
    unsigned b,e, overhead;
    // instruction prefetch seems to be 16 bytes.  
    //
    // this alignment will keep the two cycle_cnt_reads() in a single 
    // 16-byte prefetch buffer fetch.
    asm volatile(".align 4");
    b = cycle_cnt_read();
    e = cycle_cnt_read();
    overhead = e-b;
    // this should not fail.
    demand(overhead == 8, "expected 8, have overhead=%d\n", overhead);
    output("\taligned success: expected 8 cycles, got 8 cycles\n");

    // instruction prefetch seems to be 16 bytes.  so we push the two 
    // cycle count reads into different prefetches
    asm volatile(".align 4");
    asm volatile("nop");    // 4 bytes
    asm volatile("nop");    // 4 bytes
    asm volatile("nop");    // 4 bytes
    b = cycle_cnt_read();   // last 4 bytes of prefetch
    e = cycle_cnt_read();
    overhead = e-b;

    // this should fail
    if(overhead != 8)
        output("\tnon-align success: expected > 8, have overhead of = %d cycles\n", overhead);
    else
        output("\tnon-align fail: expected > 8, have overhead of 8\n");
}
