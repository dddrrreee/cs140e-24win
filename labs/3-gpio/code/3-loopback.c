// tests that your gpio_input works.
// hardware:
//   1. connect pin 18 and 19 using a jumper ("loopback").
//   2. leave your LEDs connected to 20 and 21.
// 
// we do "true" embedded system debugging without any printk
// available by using LEDs to show what the system is doing:
//  iterate, picking a value for v.
//  - set LED on 20 to v (ground truth)
//  - 1. set 18 to v.  
//    2. set LED 21 to the value read from 19.
//
//  - if both LEDs match (both on at the same time, both off
//    at the same time) you are correct.  
//   - if not: first try the staff binary to make sure your 
//     pi is wired correctly.
#include "rpi.h"

void notmain(void) {
    const int led0 = 20;
    const int led1 = 21;
    const int output = 18;
    const int input = 19;

    gpio_set_output(led0);
    // do in the middle in case some interference
    gpio_set_input(input);
    gpio_set_output(led1);
    gpio_set_output(output);

    unsigned v = 0;
    for(int i = 0; i < 20; i++) {
        // ground truth: led1 should match if gpio works.
        gpio_write(led0, v);

        // see that we get the same with loopback
        gpio_write(output, v);
        gpio_write(led1, gpio_read(input));

        // hold so can see visually (humans are slow)
        delay_cycles(1500000);
        v = !v; // could also do  <v = 1-v;>
    }
}
