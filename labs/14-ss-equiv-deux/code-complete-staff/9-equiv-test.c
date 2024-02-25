// run tests that should give the same result for everyone.
//   1. all the functions we run and hash their registers are defined
//      in staff-start.S in this directory.
//   2. everyone's assembler will convert this to the same machine code
//      (as far as i have seen)
//   3. we link this file as the first one in every binary.
//   4. therefore: the code will be at the same location for everyone.
//   5. therefore: the hashes will be the same --- same instructions, at 
//      same location, run on same initial state = same result.
//   this use of determinism let's us cross check our results across
//   everyone and across time.
//
//   fwiw, determinism tricks like this have gotten me/my group a lot 
//   of good results.  keep an eye out for such things as you go through
//   life.

#include "rpi.h"
#include "equiv-threads.h"

// lie about arg so we don't have to cast.
void nop_10(void *);
void nop_1(void *);
void mov_ident(void *);
void small1(void *);
void small2(void *);

// runs routines <N> times, comparing the result against <hash>.
// they are stateless, so each time we run them we should get
// the same result.  note: if you set hash to 0, it will run,
// and then write the result into the thread block.
static eq_th_t * 
run_single(int N, void (*fn)(void*), void *arg, uint32_t hash) {
    let th = equiv_fork_nostack(fn, arg, hash);
    th->verbose_p = 1;
    equiv_run();
    trace("--------------done first run!-----------------\n");

    // turn off extra output
    // th->verbose_p = 0;
    for(int i = 0; i < N; i++) {
        equiv_refresh(th);
        equiv_run();
        trace("--------------done run=%d!-----------------\n", i);
    }
    return th;
}

void notmain(void) {
    equiv_init();

    //  strategy: i would run just the first routine first and comment out
    //  the rest.  make sure it works.  then do the next, then the next etc.

    // do the smallest ones first.
    let th1 = run_single(3, small1, 0, 0x70916294);
    let th2 = run_single(3, small2, 0, 0x18e43fb1);
    // these calls re-initialize the threads and put them on the 
    // run queue so they can be rerun.
    equiv_refresh(th1);
    equiv_refresh(th2);
    // run all threads till run queue empty.
    equiv_run();
    trace("easy no-stack passed\n");

    // do a bunch of other ones
    let th_nop1 = run_single(3, nop_1, 0, 0xf7e37b42);
    let th_mov_ident = run_single(3, mov_ident, 0, 0x984d6fbf);
    let th_nop10 = run_single(3, nop_10, 0, 0xf5ae770b);

    // now run all three 
    equiv_refresh(th_nop1);
    equiv_refresh(th_nop10);
    equiv_refresh(th_mov_ident);
    equiv_run();

    trace("second no-stack passed\n");

    // run all together: interleaving all 5 threads.
    equiv_refresh(th1);
    equiv_refresh(th2);
    equiv_refresh(th_nop1);
    equiv_refresh(th_nop10);
    equiv_refresh(th_mov_ident);
    equiv_run();

    trace("all no-stack passed\n");

    clean_reboot();
}
