#ifndef __STACK_T_H__
#define __STACK_T_H__

// engler, 140e: simple generic constant size stack 
// can extend for variable without much trouble but don't need atm.  
//
//  gen_stack_T(pfx, stack_T, E_T, N)
//      - all method names are prefixed with "<pfx>_"
//      - <gen_stack_T> defines the type <stack_T>
//      - client defines <E_T> the element type of <stack_T>.
//      - element count: <N>

#define gen_stack_T(pfx, stack_T, E_T, N)                               \
    typedef struct {                                                    \
		E_T stack[N];                                                   \
        unsigned cnt;                                                   \
    } stack_T;                                                          \
		                                                                \
    /* this will be a big copy.  i don't know if it matters. */         \
    static inline stack_T pfx ## _mk(void) {                            \
        return (stack_T) { .cnt = 0 };                                           \
    }                                                                   \
		                                                                \
    static inline E_T *pfx ## _push_ptr(stack_T *s) {                    \
        if(s->cnt  > N)                                                 \
            panic("have %d elements, max is %d\n", s->cnt, N);          \
        return &s->stack[s->cnt++];                                     \
    }                                                                   \
		                                                                \
    static inline E_T *pfx ## _push(stack_T *s, E_T e) {                \
        E_T *p = pfx ## _push_ptr(s);                                   \
        *p = e;                                                         \
        return p;                                                       \
    }                                                                   \
		                                                                \
    static inline E_T * pfx ## _pop_ptr(stack_T *s) {                   \
        if(s->cnt == 0)                                                 \
            return 0;                                                   \
        assert(s->cnt > 0);                                             \
        return &s->stack[--s->cnt];                                     \
    }                                                                   \
		                                                                \
    static inline E_T pfx ## _pop(stack_T *s) {                         \
        E_T *e = pfx ## _pop_ptr(s);                                    \
        demand(e, attempting to pop and empty stack!);                  \
        return *e;                                                      \
    }                                                                   \
    static inline unsigned pfx ## _cnt(stack_T *s) {                    \
        return s->cnt;                                                  \
    }                                                                   \
		                                                                \
    static inline E_T *pfx ## _idx(stack_T *s, unsigned idx) {          \
        assert(idx < N);                                                \
        return &s->stack[idx];                                          \
    }                                                                   \
		                                                                \
    /*                                                                  \
     * might be better to not expose the implementation                 \
     * but ftm we use an interator that would only work for an array.   \
     */                                                                 \
    static inline E_T *pfx ## _first(stack_T *s) {                      \
        return s->cnt == 0 ? 0 : &s->stack[0];                          \
    }                                                                   \
    static inline E_T *pfx ## _last(stack_T *s) {                        \
        return s->cnt == 0 ? 0 : &s->stack[s->cnt-1];                   \
    }                                                                   \
    static inline E_T * pfx ## _next(stack_T *s, E_T *e) {              \
        return (e == pfx ## _last(s)) ? 0 : e+1;                         \
    }                                                                   \
    static inline E_T * pfx ## _prev(stack_T *s, E_T *e) {              \
        return (e == pfx ## _first(s)) ? 0 : e-1;                       \
    }
#endif

