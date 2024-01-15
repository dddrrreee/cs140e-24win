#ifndef __STACK_T_H__
#define __STACK_T_H__
// engler, 140e: simple generic constant size stack 
// can extend for variable without much trouble but don't need atm.  
//
//  gen_stack_T(pfx, E, N)
//      - prefixes all names with <pfx>
//      - stack element type: <E>
//      - element count: N

// construct the type name.
#define STACK_T(pfx) pfx ## _stack_T

#define gen_stack_T(pfx, E, N)                                       \
    typedef struct {                                                    \
		E stack[N];                                                     \
        unsigned cnt;                                                   \
    } STACK_T(pfx);                                                     \
		                                                                \
    /* this will be a big copy.  i don't know if it matters. */         \
    static inline STACK_T(pfx) pfx ## _mk(void) {                       \
        return (STACK_T(pfx)) { };                                      \
    }                                                                   \
		                                                                \
    static inline void pfx ## _push(STACK_T(pfx) *s, E e) {             \
        if(s->cnt  > N)                                                 \
            panic("have %d elements, max is %d\n", s->cnt, N);    \
        s->stack[s->cnt++] = e;                                         \
    }                                                                   \
		                                                                \
    static inline E * pfx ## _pop_ptr(STACK_T(pfx) *s) {                \
        if(s->cnt == 0)                                                 \
            return 0;                                                   \
        assert(s->cnt > 0);                                             \
        return &s->stack[--s->cnt];                                     \
    }                                                                   \
		                                                                \
    static inline E pfx ## _pop(STACK_T(pfx) *s) {                      \
        E *e = pfx ## _pop_ptr(s);                                      \
        demand(e, stack is empty!);                                     \
        return *e;                                    \
    }                                                                   \
    static inline unsigned pfx ## _cnt(STACK_T(pfx) *s) {               \
        return s->cnt;                                                  \
    }                                                                   \
		                                                                \
    /*                                                                  \
     * might be better to not expose the implementation                 \
     * but ftm we use an interator that would only work for an array.   \
     */                                                                 \
    static inline E *pfx ## _first(STACK_T(pfx) *s) {                   \
        return s->cnt == 0 ? 0 : &s->stack[0];                          \
    }                                                                   \
    static inline E *pfx ## _end(STACK_T(pfx) *s) {                     \
        return s->cnt == 0 ? 0 : &s->stack[s->cnt-1];                   \
    }                                                                   \
    static inline E * pfx ## _next(STACK_T(pfx) *s, E *e) {             \
        return (e == pfx ## _end(s)) ? 0 : ++e;                         \
    }

#error "what"
#endif
