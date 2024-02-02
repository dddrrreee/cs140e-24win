// two thiings:
//    1. <log_example()>: example of remapping gpio_write to 
//       see when it occurs.
//    2. <bitbang_uart_put8()>: implement a bit banged uart 
//       put8 to see that your timings are accurate "enough".
#include "rpi.h"
#include "wait-routines.h"

/*********************************************************
 * pull the logging stuff out into global routines: this will
 * lead to more measurment error (compiler generally can't 
 * generate code for global vars that will be as fast as if
 * they were locals).
 */

// NOTE: can actually include these as nested functions.
enum { TL_MAX_ENT = 32 };
static struct tlog {
    uint32_t cyc;
    uint32_t v;         // track the values: adds error, though.
} time_log[TL_MAX_ENT], *lp = 0;

static inline void time_log_init(void) {
    lp = &time_log[0];
}
static inline void time_log_add(unsigned v) {
    unsigned cyc = cycle_cnt_read();
    *lp++ = (struct tlog) { .cyc = cyc, .v = v };
}

// called to print out the error rate: <ncycles> is the
// target number of cycles per sample.
static inline void 
time_log_emit(const char *msg, unsigned ncycles) {
    uint32_t abs(int x) { return x < 0 ? -x : x ; }

    unsigned n = lp - &time_log[0];
    assert(n < TL_MAX_ENT);

    output("%s: time log has %d entries:\n", msg,n);

    unsigned tot_err = 0;;

    // compute each measurement's error, and add it to the cumulative
    // error total.
    // 
    // the "expected" answer is the number of cycles we were supposed
    // to delay <ncycles> times the round number <i>
    unsigned start = time_log[0].cyc;

    output("\t0: v=%d: [value of first reading]\n", time_log[0].v);

    for(int i = 1; i < n; i++) {
        // 1 measured cycles from start.
        uint32_t measured = time_log[i].cyc - start;
        // 2. perfectly accurate cycles from start.
        uint32_t expected = (i)*ncycles;

        // 3. error is just the absolute difference between the two.
        unsigned err = abs(measured - expected);

        // 4. total error accumulates
        tot_err += err;

        output("\t%d: v=%d: measured = %d, expected %d, err = %d, tot err = %d\n", 
            i, time_log[i].v, measured, expected, err, tot_err);
    }
    output("total error = %d cycles, out of %d total cycles\n", 
        tot_err, n*ncycles);

    if(tot_err > 1000)
        panic("%d cycles seems like too much error\n", tot_err);
    else
        trace("SUCCESS: low enough cycles used\n");

    output("----------------------------------------------------\n");
}

/*****************************************************************
 * example of overriding gpio_write (can do gpio_read too) to 
 * check the times.
 */

// log when we do log the time when we do gpio_write.
static inline void log_write(unsigned pin, unsigned v) {
    time_log_add(v);
    gpio_write(pin,v);
}

// low-tech, not super accurate example of how to easily
// track the cycles gpio_write() happens at.
//
// you should adapt this so that you can check your sw_uart_put8()
void log_example(void) {
    enum { ncycles = 1000};
    unsigned pin = 20, v = 0;

    output("----------------------------------------------------\n");
    output("example of remapping gpio_write(): will see more error than in <0-test-delay.c>\n");

    gpio_set_output(pin);

    time_log_init();

    // remap gpio_write to log_write 
#   define gpio_write log_write

    uint32_t start = cycle_cnt_read();

    wait_ncycles_exact(start, 1*ncycles); // 1
    gpio_write(pin, v);
    v = !v;

    wait_ncycles_exact(start, 2*ncycles); // 2
    gpio_write(pin, v);
    v = !v;

    wait_ncycles_exact(start, 3*ncycles); // 3
    gpio_write(pin, v);
    v = !v;

    wait_ncycles_exact(start, 4*ncycles); // 4
    gpio_write(pin, v);
    v = !v;

    wait_ncycles_exact(start, 5*ncycles); // 5
    gpio_write(pin, v);
    v = !v;

    wait_ncycles_exact(start, 6*ncycles); // 6
    gpio_write(pin, v);
    v = !v;

    wait_ncycles_exact(start, 7*ncycles); // 7
    gpio_write(pin, v);
    v = !v;

    wait_ncycles_exact(start, 8*ncycles); // 8
    gpio_write(pin, v);
    v = !v;

    wait_ncycles_exact(start, 9*ncycles); // 9
    gpio_write(pin, v);
    v = !v;

    time_log_emit("done with remap example", ncycles);

    // unmap it.
#   undef gpio_write 
}

/*********************************************************************
 * implement bitbang_uart.  with a not-tuned implementation i got:
 *
    sw uart: cycles per bit=6076 for baud=115200
    done with sw-uart example: time log has 10 entries:
	    0: v=0: [value of first reading]
	    1: v=0: measured = 6137, expected 6076, err = 61, tot err = 61
	    2: v=0: measured = 12224, expected 12152, err = 72, tot err = 133
	    3: v=0: measured = 18275, expected 18228, err = 47, tot err = 180
	    4: v=1: measured = 24315, expected 24304, err = 11, tot err = 191
	    5: v=0: measured = 30424, expected 30380, err = 44, tot err = 235
	    6: v=1: measured = 36503, expected 36456, err = 47, tot err = 282
	    7: v=1: measured = 42585, expected 42532, err = 53, tot err = 335
	    8: v=0: measured = 48661, expected 48608, err = 53, tot err = 388
	    9: v=1: measured = 54795, expected 54684, err = 111, tot err = 499
    total error = 499 cycles, out of 60760 total cycles
 */

#include "sw-uart.h"

#   define gpio_write log_write

static inline void 
bitbang_uart_put8(sw_uart_t *sw, uint8_t byte) {
    unsigned pin = sw->tx;
    unsigned n = sw->cycle_per_bit;

    unsigned u = n;
    unsigned s = cycle_cnt_read();

    // start bit (default is 1, so start = writing 0.
    gpio_write(pin,0); wait_ncycles_exact(s,u);  u +=n;

    // the byte.
    gpio_write(pin, (byte>> 0)&1); wait_ncycles_exact(s, u);  u +=n;
    gpio_write(pin, (byte>> 1)&1); wait_ncycles_exact(s, u);  u +=n;
    gpio_write(pin, (byte>> 2)&1); wait_ncycles_exact(s, u);  u +=n;
    gpio_write(pin, (byte>> 3)&1); wait_ncycles_exact(s, u);  u +=n;
    gpio_write(pin, (byte>> 4)&1); wait_ncycles_exact(s, u);  u +=n;
    gpio_write(pin, (byte>> 5)&1); wait_ncycles_exact(s, u);  u +=n;
    gpio_write(pin, (byte>> 6)&1); wait_ncycles_exact(s, u);  u +=n;
    gpio_write(pin, (byte>> 7)&1); wait_ncycles_exact(s, u);  u +=n;

    // stop bit (has to be 1 for at least n cycles)
    gpio_write(pin,1); wait_ncycles_exact(s,u); 
}

#   undef gpio_write

// hacked setup to test a sw_uart implemntation
void sw_uart_example(void) {
    unsigned baud = 115200;
    unsigned ncycles = (700*1000*1000)/baud;

    output("sw uart: cycles per bit=%d for baud=%d\n", ncycles, baud);
    time_log_init();

    // hand-fill in some of the stuff.
    sw_uart_t sw =  { .tx = 20, .cycle_per_bit = ncycles };
    bitbang_uart_put8(&sw, 'h');

    time_log_emit("done with sw-uart example", ncycles);
}

void notmain(void) {
    log_example();
    sw_uart_example();
    return;
}
