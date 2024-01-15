#ifndef __LIST_T_H__
#define __LIST_T_H__

// engler, cs140e: brain-dead generic list with a single next pointer.
// 
// - pfx: prepend to all helper routines.
// - E_T: the type name of the linked struct
// - <next>: name of field to hold the next element
#define gen_list_T(pfx, E_T, next_name)     \
                                                                        \
    /* remove from front of list. */                                    \
    static inline E_T *pfx ## _pop(E_T **l) {                           \
        demand(l, bad input);                                           \
        if(!*l)                                                         \
            return 0;                                                   \
        E_T *e = *l;                                                    \
        *l = e->next_name;                                              \
        return e;                                                       \
    }                                                                   \
                                                                        \
    /* insert at head (for LIFO) */                                     \
    static inline void pfx ## _push(E_T **l, E_T *e) {                  \
        e->next_name = *l;                                              \
        *l = e;                                                         \
    }                                                                   \
    static inline void pfx ## _init(E_T **l) { *l = 0; }                \
    static inline E_T pfx ## _mk(void) { return (E_T){}; }              \
                                                                        \
    /* iterator support. */                                             \
    static E_T *pfx ## _first(E_T *l) { return l; }                     \
    static E_T *pfx ## _next(E_T *e)  { return e ? e->next_name : 0; }

#endif
