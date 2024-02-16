// engler: simplistic mpu6050 accel driver code.  
//   - <mpu-6050.h> has the interface description.
//   - <mpu-6050.c> is where your code will go.
//
// simple driver that:
//    1. initializes i2c and the 6050 IMU (mpu6050_reset)
//    2. prints does repeated readings (<accel_rd>)
//
// obvious extensions:
//    1. validating that the "data is ready" check exactly matches
//       our expected rate: repeatedly reading stale values and/or 
//       losing them will distort calculations.
//    2. device interrupts to pull readings into circular queue. 
//    3. roll your own i2c.
//    4. if you have (3): then do multiple IMU's.  having an imu glove
//       is a cool final project.
//
// KEY: document why you are doing what you are doing.
//  **** put page numbers for any device-specific things you do ***
//  **** put page numbers for any device-specific things you do ***
//  **** put page numbers for any device-specific things you do ***
//  **** put page numbers for any device-specific things you do ***
//  **** put page numbers for any device-specific things you do ***
//  **** put page numbers for any device-specific things you do ***
//  **** put page numbers for any device-specific things you do ***
// 
// also: a sentence or two will go a long way in a year when you want 
// to re-use the code.
#include "rpi.h"
#include "mpu-6050.h"

// derive accel samples per second: do N readings and divide by the 
// time it took.
void accel_sps(accel_t *h) {
    // can comment this out.  am curious what rate we get for everyone.
    output("computing samples per second\n");
    uint32_t sum = 0, s, e;
    enum { N = 256 };
    for(int i = 0; i < N; i++) {
        s = timer_get_usec();
        imu_xyz_t xyz_raw = accel_rd(h);
        e = timer_get_usec();

        sum += (e - s);
    }
    uint32_t usec_per_sample = sum / N;
    uint32_t hz = (1000*1000) / usec_per_sample;
    output("usec between readings = %d usec, %d hz\n", usec_per_sample, hz);
}

void notmain(void) {
    delay_ms(100);   // allow time for i2c/device to boot up.
    i2c_init();      // can change this to use the <i2c_init_clk_div> for faster.
    delay_ms(100);   // allow time for i2c/dev to settle after init.

    // from application note.
    uint8_t dev_addr = 0b1101000;

    enum { 
        WHO_AM_I_REG      = 0x75, 
        WHO_AM_I_VAL = 0x68,       
    };

    uint8_t v = imu_rd(dev_addr, WHO_AM_I_REG);
    if(v != WHO_AM_I_VAL)
        panic("Initial probe failed: expected %b (%x), have %b (%x)\n", 
            WHO_AM_I_VAL, WHO_AM_I_VAL, v, v);
    printk("SUCCESS: mpu-6050 acknowledged our ping: WHO_AM_I=%b!!\n", v);

    // hard reset: the device won't magically reset when your pi reboots.
    mpu6050_reset(dev_addr);

    // part 1: get the accel working.

    // initialize.
    accel_t h = mpu6050_accel_init(dev_addr, accel_2g);
    assert(h.g==2);
    
    // if you want to compute samples per second.
    accel_sps(&h);

    for(int i = 0; i < 100; i++) {
        imu_xyz_t xyz_raw = accel_rd(&h);
        output("reading %d\n", i);
        xyz_print("\traw", xyz_raw);
        xyz_print("\tscaled (milligaus: 1000=1g)", accel_scale(&h, xyz_raw));

        delay_ms(1000);
    }
}
