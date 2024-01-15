#ifndef __QUEUE_T_H__
#define __QUEUE_T_H__

// engler, cs140e: brain-dead generic queue. 
//
// head/tail queue, with a single next pointer.
// 
// client provides:
//   - <pfx>: the prefix to prepend to all helper routines.
//   - E_T: the type of the queue element 
//   - <next>: name of field to hold the next element
//
// we then define all the helpers and the Q type, (<pfx_T>) which contains:
//   - <head>: name of field to hold head.
//   - <tail>: name of field to hold tail.
//
// could add a counter back.  dunno if useful.
#define Q_T(pfx) pfx ## _T

#define gen_queue_T(pfx, E_T, next)                                     \
    typedef struct {                                                    \
        E_T *head, *tail;                                               \
    } Q_T(pfx);                                                         \
    static inline Q_T(pfx) pfx ## _mk(void) {                           \
        return (Q_T(pfx)) {};                                             \
    }                                                                   \
                                                                        \
    /* iterator support. */                                             \
    static inline E_T *pfx ## _start(Q_T(pfx) *q) { return q->head; }   \
    static inline E_T *pfx ## _next(E_T *e)  { return e->next; }   \
                                                                        \
    static int inline pfx ## _empty(Q_T(pfx) *q)  {                          \
        if(q->head)                                                \
            return 0;                                                   \
        demand(!q->tail, invalid Q);                               \
        return 1;                                                       \
    }                                                                   \
                                                                        \
    /* remove from front of list. */                                    \
    static inline E_T *pfx ## _pop(Q_T(pfx) *q) {                            \
        demand(q, bad input);                                           \
        E_T *e = q->head;                                          \
        if(!e) {                                                        \
            assert(pfx ## _empty(q));                                   \
            return 0;                                                   \
        }                                                               \
        q->head= e->next;                                    \
        if(!q->head)                                               \
            q->tail= 0;                                           \
        return e;                                                       \
    }                                                                   \
                                                                        \
    /* insert at tail. (for FIFO) */                                    \
    static inline void pfx ## _append(Q_T(pfx) *q, E_T *e) {                 \
        e->next = 0;                                                    \
        if(!q->tail)                                                    \
            q->head= q->tail = e;                                      \
        else {                                                          \
            q->tail->next = e;                                          \
            q->tail= e;                                                \
        }                                                               \
    }                                                                   \
                                                                        \
    /* insert at head (for LIFO) */                                     \
    static inline void pfx ## _push(Q_T(pfx) *q, E_T *e) {                   \
        e->next = q->head;                                              \
        q->head = e;                                                    \
        if(!q->tail)                                                    \
            q->tail = e;                                                \
    }

#error "update this"
#endif
