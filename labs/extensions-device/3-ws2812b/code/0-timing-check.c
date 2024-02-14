// engler: timing checks for the ws2812b code.  requires that you implement:
//      - delay_ncycles
//      - t1h, t1l, t0h, t0l 
//
// The WS2812b requires nanosecond accurate writes; it's easy to mess these up with
// random changes.  The code below attempts to performance check that the code
// workes within its budget.
//
// note: the code can't be compiled in a seperate module since it needs inlined
// functions, so we just include the whole thing at the end of the neopixel.c file.
// 
// some problems:
//  1. obviously monitoring changes timing, so we never really know *exactly* how 
//  long something takes.  
//
//  2. while we detect if the routines the code calls has different timings, these
//  calls are to *copies* of the code (b/c of inlining).  the neopixel code calls 
//  its own copy of the routine (again: inlining).  as a result, our "checks" can 
//  pass routines that will fail when used by neopixel.  e.g., b/c their alignment
//  differs, changinging timing. 
// 
// partial solution to the above: you could use a second pi and hard poll on the pins, 
// recording when/how long the pin was hi/low.   this primitive digital analyzer is 
// actually easy to build, and useful for other things.   cs140e and cs240lx show how
// to build one that beats a $400 sanae
#include "rpi.h"
#include "WS2812B.h"
#include "neopixel.h"
#include "cycle-count.h"

// the pin used to control the light strip.
enum { pix_pin = 21 };

// DO NOT DELETE *inline*.
// you *HAVE* to inline, otherwise its a function pointer call.
// duh.
static inline void 
time_check(const char *fn, int pin, void (*fp)(unsigned), int exp) { 

    asm_align(3);
	unsigned s = cycle_cnt_read();
	unsigned e = cycle_cnt_read();
	unsigned overhead = e - s;
	demand(overhead==8, "expected 8 have: %d\n", overhead);

	dev_barrier();
    asm_align(3);
	s = cycle_cnt_read();
		fp(pin);
	e = cycle_cnt_read();
	dev_barrier();
	unsigned t = e - s - 8;

	// +/- 10 cycles seems right?
	// const unsigned tol = 10;
	// wtf, why so high?
    int tol = 20;
	printk("%s: expected %u cycles, got %u cycles\n", fn, exp, t);
	assert(t <= (exp + tol) && t >= (exp - tol));
}

// gcc bug work around 
static void checkt0l(unsigned);
static void checkt0h(unsigned);
static void checkt1h(unsigned);
static void checkt1l(unsigned);
static void checktreset(unsigned); 

#define get_raw_time() *(volatile unsigned *)0x20003004
static void time_usec(int pin) {

#define run(str, f,exp,pin) do {			    \
    asm volatile (".align 4");                  \
	unsigned b = get_raw_time();		        \
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
						\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
						\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
						\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
						\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
						\
	unsigned e = get_raw_time();		\
	printk("%s: 100 check: expected %u ns, got %u ns\n", str, exp, (e-b) * 1000/50);	\
  } while(0)

#if 0
	T0H = to_cycles(350),	// Width of a 0 bit in ns
	T1H = to_cycles(900),	// Width of a 1 bit in ns
	T0L = to_cycles(900),	// Width of a 0 bit in ns
	T1L = to_cycles(350), 	// Width of a 1 bit in ns
	FLUSH = to_cycles(50 *1000); // Width of the low gap between bits to cause a frame to latch
#endif
	
	checkt0h(pin);
	checkt0l(pin);

	checkt1h(pin);
	checkt1l(pin);
	checktreset(pin); 
}

static void checkt0h(unsigned pin) {
	run("t0h", t0h,350,pin);
}

static void checkt0l(unsigned pin) {
	run("t0l", t0l,900,pin);
}

static void checkt1h(unsigned pin) {
	run("t1h", t1h,900,pin);
}

static void checkt1l(unsigned pin) {
	run("t1l", t1l,350,pin);
}
static void checktreset(unsigned pin) {
	run("treset", treset, 50000,pin);
}

void check_timings(int pin, int die_on_error_p) {
    asm_align(3);
	unsigned s = cycle_cnt_read();
	unsigned e = cycle_cnt_read();
	unsigned overhead = e - s;
	printk("overhead = %u\n", overhead);
    assert(!die_on_error_p || overhead == 8);

	run("gpio_off_raw", gpio_set_off_raw, 22, pin);
	run("gpio_on_raw", gpio_set_on_raw, 22, pin);

	s = cycle_cnt_read();
		delay_ms(1000);
	e = cycle_cnt_read();
	unsigned t= (e - s) / (1000 * 1000);
	printk("1sec should take about 700M cycle: %u, %u\n", e - s,t);
	// should check a tighter tol (say 50k?)
    if(die_on_error_p)
	    assert(t >= MHz-1 && t <= MHz +1);

	time_check("t1h", pin, t1h, T1H);
	time_check("t0h", pin, t0h, T0H);
	time_check("t1l", pin, t1l, T1L);
	time_check("t0l", pin, t0l, T0L);
	time_check("treset", pin, treset, FLUSH);

	time_usec(pin);
}

// spell out how to check timing
static void check_delay(unsigned delay) {
    output("checking delay_ncycles(%d)\n", delay);

    // even doing nothing has overhead (of course)
	unsigned s = cycle_cnt_read();
	unsigned e = cycle_cnt_read();
    unsigned overhead = e - s;
    output("\toverhead = %d\n", overhead);
    
	s = cycle_cnt_read();
        delay_ncycles(s, delay);
	e = cycle_cnt_read();

    unsigned slop = 16;  // allow an extra 16 cycles.
    unsigned allowed_cycles = delay + overhead + slop;
    unsigned tot = e - s;
    if(tot > allowed_cycles)
        panic("delay_ncycles(%d) took %d cyc, expected at most %d\n",
            delay, tot, allowed_cycles);

    output("\tSUCCESS: delay_ncycles(%d) took %d cycle, allowed %d\n", 
            delay, tot, allowed_cycles);
}


void notmain(void) {
    caches_enable();
    gpio_set_output(pix_pin);

    // check that your delay works.
    check_delay(1000);
    check_delay(100);
    check_delay(10);
    check_delay(1);
    
#if 0
    // if you want to do more checking
    for(unsigned i = 0; i < 16; i++)
        check_delay(rpi_rand32() % 2048);
#endif

    // now use a refactored routine that shows how to use function pointers.
    output("about to check full timings\n");
    check_timings(pix_pin,0);
    output("rechecking with die on error = 1\n");
    check_timings(pix_pin,1);
    trace("SUCCESS!\n");
}
