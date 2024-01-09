// simple use of queue --- create two simulatenous queues, store,
// ane make sure that the contents are the same.
#include "libunix.h"
#include "queue-T.h"

    

struct proc { 
    struct proc *next;
    int x; 
};
gen_queue_T(runq, struct proc, next)

struct elem {
    struct elem *anything;
    int x;
};

gen_queue_T(list, struct elem, anything)

int main(void) {
    output("about to test\n");
    
    runq_T rq = runq_mk();
    list_T l = list_mk();
    
    for(unsigned i = 0; i < 10; i++) {
        struct proc *p = calloc(1, sizeof *p);
        p->x = i;
        runq_append(&rq, p);

        struct elem *e = calloc(1, sizeof *e);
        e->x = i;
        list_append(&l, e);
    }
    for(unsigned i = 0; i < 10; i++) {
        struct proc *p = runq_pop(&rq);
        output("p->x=%d\n", p->x);

        struct elem *e = list_pop(&l);
        assert(e->x == p->x);
    }
    return 0;
}
