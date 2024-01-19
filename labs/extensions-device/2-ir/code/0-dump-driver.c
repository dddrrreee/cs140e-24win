// do a raw dump of all the values for a given keypress. 
//
// this is useful to seeing how your remote behaves.
#include "rpi.h"

// spin in a loop while(gpio_read(pin) == v) until either:
//   1. the pin changes (return the number of usec passed)
//   2. <timeout> is exceeded (return 0).
static int read_while_eq(int pin, int v, unsigned timeout) {
    unsigned start = timer_get_usec_raw();
    while(1) {
        // make sure always return != 0
        if(gpio_read(pin) != v)
            return timer_get_usec_raw() - start + 1;
        // unless timeout.
        if((timer_get_usec_raw() - start) >= timeout)
            return 0;
    }
}

void notmain(void) {
    enum { 
        input = 21,
        N = 10,          // total readings
        timeout = 40000, // timeout in usec we wait to get a reading
     };

    gpio_set_input(input);
    gpio_set_pullup(input);     // default = 1, so we pull up.
    assert(gpio_read(input) == 1);

    output("will try to do a raw dump of %d readings\n", N);
    for(int i = 0; i < N; i++) {
        output("trial %d: about to read\n", i);

        // again: default is 1, so nothing is happening.
        while(gpio_read(input) == 1)
            ;

        // read values until timeout.  the values should alternate
        // (v starts = 0, then goes to 1 then 0 etc)
        uint32_t val[255], idx,v;
        v = 0;
        idx=0;
        while(idx < 255) {
            uint32_t t = read_while_eq(input, v, timeout);
            if(!t)
                break;
            val[idx++] = t;
            v = 1 - v;
        }
        val[idx+1] = 0;

        // print them out, two at a time so it's easy to see whats going
        // on.
        for(unsigned j = 0; j < idx; j+=2) 
            output("%d: v=0: usec=%d v=1, usec=%d\n", j, val[j], val[j+1]);
    }
}
