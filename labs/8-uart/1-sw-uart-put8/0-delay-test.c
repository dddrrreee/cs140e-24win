#include "rpi.h"
#include "wait-routines.h"

// force alignment so have cleaner timing.
#define align()  asm volatile(".align 4")

// no op instruction.
#define nop()   asm volatile("nop");

void nop25_no_inline(void);
void busy_work(void);
unsigned work_ncycles = 20;

// can have this do stuff, to increase overhead.
static inline void nop25_fn(void) {
    nop(); nop(); nop(); nop(); nop();
    nop(); nop(); nop(); nop(); nop();
    nop(); nop(); nop(); nop(); nop();
    nop(); nop(); nop(); nop(); nop();
}

static inline void nop50_fn(void) {
    nop25_fn();
    nop25_fn();
    nop25_fn();
    nop25_fn();
    nop25_fn();
    nop25_fn();
}

static inline void nop_fn(void) { }

#if 0
// delay <ncycles> counting from <start_cycle>.  call <fn> if 
// not null.  return the actual cycles elapsed.
static inline uint32_t 
wait_ncycles_exact_fn(
    uint32_t start_cycle,       // cycle we start counting from.
    uint32_t ncycles,           // stop when start_cycle_ncycle have passed
    void (*fn)(void))     // call while spinning if not null.
{
    uint32_t c;

    while(((c = cycle_cnt_read()) - start_cycle) < ncycles) {
        if(fn)
            fn();
    }
    return c;

    todo("delay using cycle_cnt_read()");
}

// same but no fn().
static inline uint32_t 
wait_ncycles_exact(uint32_t s, uint32_t n) {
    return wait_ncycles_exact_fn(s,n,0);
}

// delay for <n> cycles: if <fn> is non-null, call it 
// when waiting.
static inline uint32_t 
wait_ncycles_fn(uint32_t ncycles, void (*fn)(void)) {
    uint32_t s = cycle_cnt_read();
    return wait_ncycles_exact_fn(s, ncycles, fn);
}

// same, but no yield.
static inline uint32_t wait_ncycles(uint32_t ncycles) {
    return wait_ncycles_fn(ncycles, 0);
}

#endif
    
static inline uint32_t abs(int32_t x) {
    return (x<0) ? -x : x;
}

// measure the absolute error between expected number
// of cycles that <fn> should take vs what it actually
// took.  
//
// note: this won't be zero b/c of measurement overhead.
#define measure_err(err, fn, expect_n) do {                     \
    /* 1. measure the actual cycles <fn> takes. */              \
    uint32_t s = cycle_cnt_read();                              \
    fn;                                                         \
    uint32_t e = cycle_cnt_read();                              \
    /* total cycles = end - start */                            \
    uint32_t measured_n = e - s;                                \
    /* 2. error is absolute value of the difference */          \
    err += abs(expect_n - measured_n);                          \
} while(0)

// measure the absolute error between expected number
// of cycles that <fn> should take vs what it actually
// took.  
//
// note: this won't be zero b/c of measurement overhead.
#define measure_(err, fn, expect_n) do {                     \
    fn;                                                         \
    *log++ = cycle_cnt_read();                                  \
} while(0)

enum { ncycles = 5000 };
enum { MAX_TRIALS = 20, ntrials = 10 };

static void 
emit_measure(const char *msg, uint32_t *cnts, uint32_t measured) {
    uint32_t exact = ncycles * ntrials;
    uint32_t err = abs(measured-exact);
    output("raw wait_ncycles = exact tot=%d, measured tot=%d tot err=%d\n",
        exact, measured, err);

    uint32_t base = cnts[0];
    for(uint32_t i = 1; i <= ntrials; i++) {
        exact = ncycles*i;  
        measured = cnts[i]-base;
        err = abs(measured-exact);
        output("   %d: measure = %d, exact =%d, err=%d\n", i, measured,exact,err);
    }
}

void measure(unsigned work) {
    output("doing experiments: busywprk=%d\n", work);
    work_ncycles = work;

    uint32_t start,end,measured,exact;
    uint32_t err = 0;

    // caches_enable();

    uint32_t cnts[MAX_TRIALS], *log;
    

    //****************************************************************
    // measure without any overhead b/n the calls.  this is the best
    // case for <wait_ncycles> but we still expect to see cumulative,
    // increasing error.
    //
    // if we did work b/n the calls will see even more.
    //
    // algorithm: store the cycle count that each call completes
    // at in a sequential log.  compute the cycles between each
    // call and compare to perfect (ncycles)
    output("about to measure wait_ncycles\n");
    log = &cnts[0];

    align();
    *log++ = start = cycle_cnt_read();

    // 5
    *log++ = wait_ncycles(ncycles);
    *log++ = wait_ncycles(ncycles);
    *log++ = wait_ncycles(ncycles);
    *log++ = wait_ncycles(ncycles);
    *log++ = wait_ncycles(ncycles);

    // 5
    *log++ = wait_ncycles(ncycles);
    *log++ = wait_ncycles(ncycles);
    *log++ = wait_ncycles(ncycles);
    *log++ = wait_ncycles(ncycles);
    *log++ = wait_ncycles(ncycles);
    end = cycle_cnt_read();

    unsigned n= log - &cnts[0];
    assert(n == ntrials+1);
    emit_measure("wait_ncycles [no work]", cnts, end-start);

#define fn 0
//#define fn nop_fn
//#define fn nop25_fn
//#define fn nop25_no_inline

#if 0
    //****************************************************************
    // same as above but add a yield call: will add overhead.
    output("------------------------------------------------\n");
    output("about to measure wait_ncycles_fn\n");
    log = &cnts[0];

    align();
    *log++ = start = cycle_cnt_read();

    // 5
    *log++ = wait_ncycles_fn(ncycles, fn);
    *log++ = wait_ncycles_fn(ncycles, fn);
    *log++ = wait_ncycles_fn(ncycles, fn);
    *log++ = wait_ncycles_fn(ncycles, fn);
    *log++ = wait_ncycles_fn(ncycles, fn);

    // 5
    *log++ = wait_ncycles_fn(ncycles, fn);
    *log++ = wait_ncycles_fn(ncycles, fn);
    *log++ = wait_ncycles_fn(ncycles, fn);
    *log++ = wait_ncycles_fn(ncycles, fn);
    *log++ = wait_ncycles_fn(ncycles, fn);
    end = cycle_cnt_read();

    emit_measure("wait_ncycles [nop fn]", cnts, end-start);
#endif

    //**********************************************************
    // should be most amount of error: spin for extra work
    // between each run.
    
    output("about to measure wait_ncycles with busywork=%d\n", work_ncycles);
    log = &cnts[0];

    align();
    *log++ = start = cycle_cnt_read();

    // 5
    busy_work(); *log++ = wait_ncycles(ncycles);
    busy_work(); *log++ = wait_ncycles(ncycles);
    busy_work(); *log++ = wait_ncycles(ncycles);
    busy_work(); *log++ = wait_ncycles(ncycles);
    busy_work(); *log++ = wait_ncycles(ncycles);

    // 5
    busy_work(); *log++ = wait_ncycles(ncycles);
    busy_work(); *log++ = wait_ncycles(ncycles);
    busy_work(); *log++ = wait_ncycles(ncycles);
    busy_work(); *log++ = wait_ncycles(ncycles);
    busy_work(); *log++ = wait_ncycles(ncycles);

    end = cycle_cnt_read();
    emit_measure("wait_ncycles [no work]", cnts, end-start);


#if 0
    //****************************************************************
    // exact waiting should not have cumulative error and should
    // much tighter than above. 
    output("------------------------------------------------\n");
    output("about to measure wait_ncycles_exact\n");
    log = &cnts[0];

    align();
    *log++ = start = cycle_cnt_read();

    // 5
    *log++ = wait_ncycles_exact(start, 1*ncycles);
    *log++ = wait_ncycles_exact(start, 2*ncycles);
    *log++ = wait_ncycles_exact(start, 3*ncycles);
    *log++ = wait_ncycles_exact(start, 4*ncycles);
    *log++ = wait_ncycles_exact(start, 5*ncycles);

    // 5
    *log++ = wait_ncycles_exact(start, 6*ncycles);
    *log++ = wait_ncycles_exact(start, 7*ncycles);
    *log++ = wait_ncycles_exact(start, 8*ncycles);
    *log++ = wait_ncycles_exact(start, 9*ncycles);
    *log++ = wait_ncycles_exact(start, 10*ncycles);

    end = cycle_cnt_read();

    emit_measure("wait_ncycles_exact [no work]", cnts, end-start);
#endif

    //****************************************************************
    // adding a function call shouldn't affect it much.
    output("------------------------------------------------\n");
    output("about to measure wait_ncycles_exact_fn\n");
    log = &cnts[0];

    align();
    *log++ = start = cycle_cnt_read();

    // 5
    *log++ = wait_ncycles_exact_fn(start, 1*ncycles, fn);
    *log++ = wait_ncycles_exact_fn(start, 2*ncycles, fn);
    *log++ = wait_ncycles_exact_fn(start, 3*ncycles, fn);
    *log++ = wait_ncycles_exact_fn(start, 4*ncycles, fn);
    *log++ = wait_ncycles_exact_fn(start, 5*ncycles, fn);

    // 5
    *log++ = wait_ncycles_exact_fn(start, 6*ncycles, fn);
    *log++ = wait_ncycles_exact_fn(start, 7*ncycles, fn);
    *log++ = wait_ncycles_exact_fn(start, 8*ncycles, fn);
    *log++ = wait_ncycles_exact_fn(start, 9*ncycles, fn);
    *log++ = wait_ncycles_exact_fn(start, 10*ncycles, fn);

    end = cycle_cnt_read();

    emit_measure("wait_ncycles_exact [no work]", cnts, end-start);


    //****************************************************************
    // add busy work: should not affect much.
    output("------------------------------------------------\n");
    output("about to measure wait_ncycles_exact_fn\n");
    log = &cnts[0];

    align();
    *log++ = start = cycle_cnt_read();

    // 5
    busy_work(); 
    *log++ = wait_ncycles_exact_fn(start, 1*ncycles, fn);
    busy_work(); 
    *log++ = wait_ncycles_exact_fn(start, 2*ncycles, fn);
    busy_work(); 
    *log++ = wait_ncycles_exact_fn(start, 3*ncycles, fn);
    busy_work(); 
    *log++ = wait_ncycles_exact_fn(start, 4*ncycles, fn);
    busy_work(); 
    *log++ = wait_ncycles_exact_fn(start, 5*ncycles, fn);

    // 5
    busy_work(); 
    *log++ = wait_ncycles_exact_fn(start, 6*ncycles, fn);
    busy_work(); 
    *log++ = wait_ncycles_exact_fn(start, 7*ncycles, fn);
    busy_work(); 
    *log++ = wait_ncycles_exact_fn(start, 8*ncycles, fn);
    busy_work(); 
    *log++ = wait_ncycles_exact_fn(start, 9*ncycles, fn);
    busy_work(); 
    *log++ = wait_ncycles_exact_fn(start, 10*ncycles, fn);


    end = cycle_cnt_read();

    emit_measure("wait_ncycles_exact [no work]", cnts, end-start);
}

void notmain(void) {
    measure(0);
    measure(10);
    measure(50);
}

void nop25_no_inline(void) {
    nop25_fn();
}

// parameterize extra work: spin for <work_ncycles>
void busy_work(void) {
    unsigned n = work_ncycles;
    for(unsigned i = 0; i < n; i++)
        nop();
}
