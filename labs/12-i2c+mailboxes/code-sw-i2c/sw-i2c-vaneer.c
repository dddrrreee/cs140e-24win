// this is a simple veneer that transparently translates the 
// hardware interface used by the MPU6050 in the last lab 
// to your software i2c.
#include "rpi.h"
#include "sw-i2c.h"
#include "i2c.h"

// the hardware pins.
enum { SCL = 3, SDA = 2 };

// there's only one hardware i2c so we just use a global variable
// to track its state.
static i2c_t hw;

void i2c_init(void) {
    // hard coded address just for today.  you'd want to set 
    // this.
    enum { addr = 0b1101000 };
    hw = sw_i2c_init(addr, SCL, SDA);
    i2c_t h = sw_i2c_init(addr, SCL, SDA);
}

// write <nbytes> of <datea> to i2c device address <addr>
int i2c_write(unsigned addr, uint8_t data[], unsigned nbytes) {
    return sw_i2c_write(&hw, data, nbytes);
}

// read <nbytes> of <datea> from i2c device address <addr>
int i2c_read(unsigned addr, uint8_t data[], unsigned nbytes) {
    return sw_i2c_read(&hw, data, nbytes);
}
