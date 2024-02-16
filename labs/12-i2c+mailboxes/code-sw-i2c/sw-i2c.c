// extensions: 
//  1. tune the different delays
//  2. make sure you handle failures and return error codes or 
//     explicitly die

#include "rpi.h"
#include "sw-i2c.h"

// Hardware-specific support functions that MUST be customized:
#define I2CSPEED 100

// can swap the two interfaces
#define USE_PULLUP
#ifdef USE_PULLUP

// put all the code that assumes the use of pullups.

// this worked for me if i used pullup
static void pin_setup(uint32_t scl, uint32_t sda) {
    gpio_set_input(scl);
    gpio_set_input(sda);
    gpio_set_pullup(scl);
    gpio_set_pullup(sda);
}
#else

// or put the code that assumes you explicitly set scl/sda

// this worked for me if i explicitly set values.
static void pin_setup(uint32_t scl, uint32_t sda) {
    gpio_set_output(scl);
    gpio_set_output(sda);
    gpio_write(scl,1);
    gpio_write(scl,1);
}

#endif

i2c_t sw_i2c_init(uint8_t addr, uint32_t scl, uint32_t sda) {
    assert(scl<32);
    assert(sda<32);
    pin_setup(scl,sda);

    return (i2c_t) { 
        .is_transmit_p = 1, 
        .SCL = scl, 
        .SDA = sda, 
        .addr = addr 
    };
}

// should let them configure.
static inline void I2C_delay(i2c_t *h) { 
    delay_us(4); 
}

int sw_i2c_write(i2c_t *h, uint8_t data[], unsigned nbytes) {
    todo("implement: make sure you send addr<<1|0!");
    return 1;
}

int sw_i2c_read(i2c_t *h, uint8_t data[], unsigned nbytes) {
    todo("implement: make sure you send addr<<1|1!");
    return 1;
}
