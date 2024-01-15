// engler: simple 140e circular buffer implementation.  should
// be thread safe for interrupt handler producer / non-interrupt
// consumer.

#ifndef __CIRCULAR_T_H__
#define __CIRCULAR_T_H__

// simple lock-free circular queue.
#ifndef RPI_UNIX
#   include "rpi.h"
#   include "rpi-interrupts.h"
#else
#   define printk printf
#   include <assert.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#   define int_is_enabled() 0
#endif

// constructs the typename given user-supplied <pfx>
//     "<pfx>_cq_T"
#define CQ_T(pfx) pfx ## _cq_T 

// single mutator of head, single mutator of tail = lock free.
//
//  - pfx: prepend to all helper routines.
//  - E is the name of the contained cicular q type (eg cqe_t)
//  - N is the static size of the queue.
#define gen_circular(pfx, E, N)                           \
    typedef struct {                                            \
        E c_buf[N];                                             \
        unsigned fence;                                         \
        unsigned head, tail;                                    \
                                                                \
        /* number of times a push failed b/c full. */           \
        unsigned overflow;                                      \
        /* =1 -> we panic on error. */                          \
        unsigned errors_fatal_p:1;                              \
    } CQ_T(pfx);                                                \
                                                                \
    static inline int pfx ## _cq_empty(CQ_T(pfx) *q) {          \
        gcc_mb();                                               \
        return q->head == q->tail;                              \
    }                                                           \
    static inline int pfx ## _cq_full(CQ_T(pfx) *q) {           \
        gcc_mb();                                               \
        return (q->head+1) % (N) == q->tail;                    \
    }                                                           \
                                                                \
    static inline unsigned pfx ## _cq_nelem(CQ_T(pfx) *q) {     \
        gcc_mb();                                               \
        return (q->head - q->tail) % (N);                       \
    }                                                           \
                                                                \
    /* not blocking: requires interrupts. */                    \
    static inline int pfx ## _cq_pop_nonblk(CQ_T(pfx) *c, E *e) {  \
        if(pfx ## _cq_empty(c))                                 \
            return 0;                                           \
                                                                \
        /* there is a memory barrier in cq_eq */                \
        unsigned tail = c->tail;                                \
        /* must occur in order. */                              \
        *e = c->c_buf[tail];     /* 1 */                        \
        gcc_mb();                                               \
        c->tail = (tail+1)% (N);  /* 2 */                       \
        gcc_mb();                                               \
        return 1;                                               \
    }                                                           \
                                                                \
    /* blocking: called from non-interrupt code. */             \
    static inline E pfx ## _cq_pop(CQ_T(pfx) *c) {              \
        E e;                                                    \
        memset(&e, 0, sizeof e);                                \
                                                                \
	    /* wait til interrupt puts something: deadlocks if  */  \
        /* ints not enabled: need to yield. */                  \
        while(!pfx ## _cq_pop_nonblk(c,&e)) {                   \
            if(!int_is_enabled())                               \
                panic("will deadlock: interrupts not enabled [FIXME]\n");    \
        }                                                       \
        return e;                                               \
    }                                                           \
                                                                \
    /* non-blocking push: returns 0 if full.*/                  \
    static inline int pfx ## _cq_push(CQ_T(pfx) *c, E x) {      \
        if(pfx ## _cq_full(c))                                  \
            return 0;                                           \
        gcc_mb();                                               \
        unsigned head = c->head;                                \
        /* must occur in order */                               \
        c->c_buf[head] = x;  /* 1 */                            \
        gcc_mb();                                               \
        c->head = (head + 1) % (N); /* 2 */                     \
        gcc_mb();                                               \
        return 1;                                               \
    }                                                           \
                                                                \
    /* not thread safe in current form: need int disable */     \
    static inline E *pfx ## _cq_get(CQ_T(pfx) *c, unsigned i) { \
        if(i >= pfx ## _cq_nelem(c))                            \
            return 0;                                           \
        unsigned idx = (c->tail + i) % N;                       \
        return &c->c_buf[idx];                                  \
    }                                                           \
                                                                \
    static inline void pfx ## _cq_push_w_drop(CQ_T(pfx) *q, E e) {    \
        /* discard oldest if full */                            \
        if(pfx ## _cq_full(q)) {                                \
            assert(!int_is_enabled());                          \
            pfx ## _cq_pop(q);                                  \
        }                                                       \
        if(! pfx ## _cq_push(q,e))                              \
            panic("impossible\n");                              \
    }                                                           \
                                                                \
    static inline CQ_T(pfx) pfx ## _cq_mk(int errors_fatal_p) { \
        CQ_T(pfx) cq = {0};                                     \
        cq.fence = 0x12345678;                                  \
        cq.errors_fatal_p = errors_fatal_p;                     \
        assert(pfx ## _cq_empty(&cq));                          \
        assert(!pfx ## _cq_full(&cq));                          \
        assert(pfx ## _cq_nelem(&cq) == 0);                     \
        return cq;                                              \
    }

#undef NAME

#endif
