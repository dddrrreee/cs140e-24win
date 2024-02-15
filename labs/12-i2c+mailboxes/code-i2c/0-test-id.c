// dumb initial test that you can read the device address.
#include "rpi.h"
#include "i2c.h"

// read a single device register <reg> from i2c device 
// <addr> and return the result.
static uint8_t i2c_get_reg(uint8_t addr, uint8_t reg) {
    i2c_write(addr, &reg, 1);

    uint8_t v;
    i2c_read(addr,  &v, 1);
    return v;
}

void notmain(void) {
    delay_ms(100);   // allow time for i2c/device to boot up.
    i2c_init();
    delay_ms(100);   // allow time for i2c/dev to settle after init.

    // from application note.
    uint8_t dev_addr = 0b1101000;

    enum {
        WHO_AM_I_REG      = 0x75,
        WHO_AM_I_VAL = 0x68,
    };

    uint8_t v = i2c_get_reg(dev_addr, WHO_AM_I_REG);
    if(v != WHO_AM_I_VAL)
        panic("Initial probe failed: expected %b (%x), have %b (%x)\n",
            WHO_AM_I_VAL, WHO_AM_I_VAL, v, v);
    printk("SUCCESS: mpu-6050 acknowledged our ping: WHO_AM_I=%b!!\n", v);
}
