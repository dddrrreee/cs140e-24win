/*
 * test for your <wait_ncycles_exact> in <wait-routines.h>
 *
 *    1. we do <N> calls to it, attempting to have exactly <ncycles> 
 *       of delay between each call.
 *    2. records the actual cycles between calls and compares.
 *
 * the error will bounce around, but when i run i get around:

	    trial=0: measured = 601, expected 600, err = 1, tot err = 1
	    trial=1: measured = 1200, expected 1200, err = 0, tot err = 1
	    trial=2: measured = 1808, expected 1800, err = 8, tot err = 9
	    trial=3: measured = 2407, expected 2400, err = 7, tot err = 16
	    trial=4: measured = 3003, expected 3000, err = 3, tot err = 19
	    trial=5: measured = 3608, expected 3600, err = 8, tot err = 27
	    trial=6: measured = 4207, expected 4200, err = 7, tot err = 34
	    trial=7: measured = 4803, expected 4800, err = 3, tot err = 37
	    trial=8: measured = 5408, expected 5400, err = 8, tot err = 45
	    trial=9: measured = 6000, expected 6000, err = 0, tot err = 45
    total error = 45 cycles, out of 6000 total cycles

 *
 */
#include "rpi.h"
#include "wait-routines.h"

void notmain(void) {
    uint32_t abs(int x) { return x < 0 ? -x : x ; }

    // trivial log of cycle counts: we use local variables to 
    // minimize overheads.
    enum { N = 100};
    uint32_t time_log[N+1], *log = &time_log[0];

    // cycles between readings: try different values!
    enum { ncycles = 600 };
    
    // try with and without the cache enabled: why the difference?
    caches_enable();

    // the base measurment every delay is in reference to.
    uint32_t start = cycle_cnt_read();
    
    // these should each be about <ncycles> between each other.
    //
    // the difference between the actual delay and <ncycles> is
    // the error.  we compute this by storing the actual cycle 
    // counter value in <log> and compare.
    *log++ = wait_ncycles_exact(start, 1*ncycles); // 1
    *log++ = wait_ncycles_exact(start, 2*ncycles); // 2
    *log++ = wait_ncycles_exact(start, 3*ncycles); // 3
    *log++ = wait_ncycles_exact(start, 4*ncycles); // 4
    *log++ = wait_ncycles_exact(start, 5*ncycles); // 5
    *log++ = wait_ncycles_exact(start, 6*ncycles); // 6
    *log++ = wait_ncycles_exact(start, 7*ncycles); // 7
    *log++ = wait_ncycles_exact(start, 8*ncycles); // 8
    *log++ = wait_ncycles_exact(start, 9*ncycles); // 9
    *log++ = wait_ncycles_exact(start, 10*ncycles); // 10

    // done: make sure didn't blow out the log.
    unsigned n = log - &time_log[0];
    assert(n<N);

    // compute each measurement's error, and add it to the cumulative
    // error total.
    // 
    // the "expected" answer is the number of cycles we were supposed
    // to delay <ncycles> times the round number <i>
    uint32_t tot_err=0;
    for(int i =0; i < n; i++) {
        // 1 measured cycles from start.
        uint32_t measured = time_log[i]-start;
        // 2. perfectly accurate cycles from start.
        uint32_t expected = (i+1)*ncycles;

        // 3. error is just the absolute difference between the two.
        uint32_t err = abs(measured - expected);

        // 4. total error accumulates
        tot_err += err;

        output("\ttrial=%d: measured = %d, expected %d, err = %d, tot err = %d\n", 
            i,measured, expected, err, tot_err);
    }
    output("total error = %d cycles, out of %d total cycles\n", 
        tot_err, N*ncycles);

    if(tot_err > 100)
        panic("%d cycles seems like too much error\n", tot_err);
    else
        trace("SUCCESS: low enough error!\n");
}
