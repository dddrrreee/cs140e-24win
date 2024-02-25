// for this test we will *not* get the same hashes, but you should get the
// same hash within a given run.  
//   1. the routines are written in C code and are in the middle of the bhinary.
//   2. they will almost certainly be at different locations given that
//      people have different implementations and different compiler versions
//      --- even a single additional instruction at the start of the binary
//      will give different results.
//   3. however, since the routines are deterministic, you should get the 
//      same results within a given run.
#include "rpi.h"
#include "equiv-threads.h"

// lie about arg so we don't have to cast.
void nop_10(void *);
void nop_1(void *);
void mov_ident(void *);
void small1(void *);
void small2(void *);

void sys_equiv_putc(uint8_t ch);

void equiv_puts(char *msg) {
    for(; *msg; msg++)
        sys_equiv_putc(*msg);
}

void hello(void *msg) {
    equiv_puts("hello from 1\n");
}
void msg(void *msg) {
    equiv_puts(msg);
}

// these threads use stacks.
static eq_th_t * run_single(int N, void (*fn)(void*), void *arg, uint32_t hash) {
    // run just the one thread by itself.
    let th = equiv_fork(fn, arg, hash);     // if hash is 0 will get assigned at end.
    // we turn off printing: if you need to debug enable it.
    th->verbose_p = 0;
    equiv_run();
    trace("--------------done first run!-----------------\n");

    // now rerun and make sure we get the same hash.
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

    // do the smallest ones first.
    let th1 = run_single(0, hello, 0, 0);
    let th2 = run_single(0, msg, "hello from 2\n", 0);
    let th3 = run_single(0, msg, "hello from 3\n", 0);

    // note: we turn off printing. if you need to debug you'd enable it.
    th1->verbose_p = 0;
    th2->verbose_p = 0;
    th3->verbose_p = 0;

    equiv_refresh(th1);
    equiv_refresh(th2);
    equiv_refresh(th3);
    equiv_run();
    trace("stack passed!\n");

    output("---------------------------------------------------\n");
    output("about to do quiet run\n");
    equiv_verbose_off();
    equiv_refresh(th1);
    equiv_refresh(th2);
    equiv_refresh(th3);
    equiv_run();
    trace("stack passed!\n");

    clean_reboot();
}
