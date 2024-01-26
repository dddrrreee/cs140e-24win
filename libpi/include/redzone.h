// really dumb redzone check of bytes [0...4096)
#ifndef __REDZONE_H__
#define __REDZONE_H__

enum { RZ_NBYTES = 4096 };

static inline int redzone_check(void) {
    volatile uint32_t *rz = (void*)0;

    unsigned nfail = 0;
    for(unsigned i = 0; i < RZ_NBYTES/4; i++) {
        if(rz[i] != 0) {
            nfail++;
            output("non-zero redzone: off=%x, val=%x fail=%d\n", i*4,rz[i], nfail);
        }
    }
    return nfail == 0;
}

static inline void redzone_init(void) {
    memset(0, 0, RZ_NBYTES);
    assert(redzone_check());
}
#endif

