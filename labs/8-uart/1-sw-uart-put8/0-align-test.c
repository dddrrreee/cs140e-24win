#include "rpi.h"
#include "cycle-count.h"

// force alignment so have cleaner timing.
#define align()  asm volatile(".align 4")

// no op instruction.
#define nop()   asm volatile("nop");

// measure the overhead of not-being 16 byte aligned.
// we already saw this a few labs ago, but just to see again with
// a few more tricks.
void measure_align(void) {
    uint32_t s,e;

    // simple examples to show alignment is has timing
    // implications.
    align();
label0:
    s = cycle_cnt_read();
    e = cycle_cnt_read();
    output("start of 16 byte block overhead      = %d cycles\n", e-s);
    uint32_t base = e-s;

    align();
    nop();
label4:
    s = cycle_cnt_read();
    e = cycle_cnt_read();
    output("4 bytes into 16-byte block overhead  = %d cycles [expect %d]\n", 
            e-s,base);

    align();
    nop();
    nop();
label8:
    s = cycle_cnt_read();
    e = cycle_cnt_read();
    output("8 bytes into 16-byte block overhead  = %d cycles [expect %d]\n", 
        e-s, base);

    align();
    nop();
    nop();
    nop();
label12:
    s = cycle_cnt_read();
    e = cycle_cnt_read();
    output("12 bytes into 16-byte block overhead = %d cycles [expect > %d]\n", 
        e-s, base);

    align();
    nop();
    nop();
    nop();
    nop();
label16:
    s = cycle_cnt_read();
    e = cycle_cnt_read();
    output("16 bytes into 16-byte block overhead = %d cycles [expect %d]\n", 
        e-s, base);

    // take the address of label (a gcc extension) and make sure that
    // the alignment is what we expect.
    assert((uint32_t)&&label0 % 16 == 0);   // 16 byte aligned
    assert((uint32_t)&&label4 % 16 == 4);  
    assert((uint32_t)&&label8 % 16 == 8);
    assert((uint32_t)&&label12 % 16 == 12);
    assert((uint32_t)&&label16 % 16 == 0);
}

void notmain(void) {
    measure_align();
}

