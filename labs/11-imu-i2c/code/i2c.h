#ifndef __RPI_I2C_H__
#define __RPI_I2C_H__

// initialize i2c: note, in many cases the i2c device has a "settling time" while it
// boots up so you have to delay before you can get legal values.  (delay often in ms)
void i2c_init(void);

// shortest will be 130 for i2c accel.
void i2c_init_clk_div(unsigned clk_div);

// can call N times, will only initialize once (the first time)
void i2c_init_once(void);

// write <nbytes> of <datea> to i2c device address <addr>
int i2c_write(unsigned addr, uint8_t data[], unsigned nbytes);
// read <nbytes> of <datea> from i2c device address <addr>
int i2c_read(unsigned addr, uint8_t data[], unsigned nbytes);


#endif
