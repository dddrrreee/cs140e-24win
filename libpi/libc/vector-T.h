#ifndef __STACK_T_H__
#define __STACK_T_H__

// engler, 140e: simple generic constant size stack 
// constant size = can stack/global allocate it without 
// dynamic allocation.
//
// can extend for variable stack size without much trouble but 
// don't need atm.  
//
//  gen_vector_T(pfx, vector_T, E_T, N)
//      - all method names are prefixed with "<pfx>_"
//      - <gen_vector_T> defines the type <vector_T>
//      - client defines <E_T> the element type of <vector_T>.
//      - element count: <N>
//
#define gen_vector_T(pfx, vector_T, E_T, N)                             \
    typedef struct {                                                    \
        unsigned cnt;                                                   \
		E_T vector[N];                                                   \
    } vector_T;                                                         \
		                                                                \
    /* this will be a big copy.  i don't know if it matters. */         \
    static inline vector_T pfx ## _mk(void) {                           \
        return (vector_T) { .cnt = 0 };                                 \
    }                                                                   \
		                                                                \
    static inline unsigned pfx ## _cnt(vector_T *v) {                   \
        return v->cnt;                                        \
    }                                                                   \
		                                                                \
	/* no bounds checking */                                            \
    static inline E_T *pfx ## _append_raw(vector_T *v, E_T e) {         \
        E_T *ptr = &v->vector[v->cnt++];                                \
        *ptr = e;                                                       \
        return ptr;                                                       \
    }                                                                   \
    static inline E_T *pfx ## _append(vector_T *v, E_T e) {             \
        assert(pfx ## _cnt(v) < N);                                     \
        return pfx ## _append_raw(v,e);                                 \
    }                                                                   \
		                                                                \
    static inline E_T *pfx ## _idx_ptr(vector_T *v, unsigned idx) {         \
        assert(idx < N);                                                \
        return &v->vector[idx];                                          \
    }                                                                   \
    static inline E_T pfx ## _idx(vector_T *v, unsigned idx) {         \
        assert(idx < N);                                                \
        return v->vector[idx];                                          \
    }                                                                   \
		                                                                \
		                                                                \
    /*                                                                  \
     * might be better to not expose the implementation                 \
     * but ftm we use an interator that would only work for an array.   \
     * actually: with copy semantics, this can cause issues unless      \
     * you are exactly iterating.                                       \
     */                                                                 \
    static inline E_T *pfx ## _first(vector_T *v) {                      \
        return v->cnt == 0 ? 0 : &v->vector[0];                          \
    }                                                                   \
    static inline E_T *pfx ## _last(vector_T *v) {                       \
        return v->cnt == 0 ? 0 : &v->vector[v->cnt-1];                   \
    }                                                                   \
    static inline E_T * pfx ## _next(vector_T *v, E_T *e) {              \
        return (e == pfx ## _last(v)) ? 0 : e+1;                        \
    }                                                                   \
    static inline E_T * pfx ## _prev(vector_T *v, E_T *e) {              \
        return (e == pfx ## _first(v)) ? 0 : e-1;                       \
    }
#endif
