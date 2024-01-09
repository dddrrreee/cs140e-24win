#include "libunix.h"
#include "list-T.h"

struct proc; 

struct proc { 
    struct proc *next;
    int x; 
};
    
gen_list_T(proc, struct proc, next);

int main(void) {
    output("about to test\n");
    
    struct proc *l = 0;

    for(unsigned i = 0; i < 10; i++) {
        struct proc *p = calloc(1, sizeof *p);
        p->x = i;
        proc_push(&l, p);
    }

    for(struct proc *e = l; e; e = proc_next(e))
        output("e->x=%d\n", e->x);

    for(unsigned i = 0; i < 10; i++) {
        struct proc *p = proc_pop(&l);
        output("p->x=%d\n", p->x);
    }

    return 0;
}
