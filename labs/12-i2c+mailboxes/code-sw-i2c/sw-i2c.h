#ifndef __SW_UART_H__
#define __SW_UART_H__

#include <stdbool.h>

typedef struct i2c {
    // is this a transmitter or receiver?
    unsigned is_transmit_p;
    bool started_p;

    uint8_t addr;
    uint8_t SCL;
    uint8_t SDA;

    // these can switch back and forth. 
    unsigned SCL_is_input_p:1;
    unsigned SDA_is_input_p:1;
} i2c_t;

i2c_t sw_i2c_init(uint8_t addr, uint32_t scl, uint32_t sda);
int sw_i2c_write(i2c_t *h, uint8_t data[], unsigned nbytes);
int sw_i2c_read(i2c_t *h, uint8_t data[], unsigned nbytes);
#endif
