#ifndef __MPU_6050_H__
#define __MPU_6050_H__

#include "i2c.h"
#include "bit-support.h"
#include <limits.h>

// both gyro and accel return x,y,z readings: it makes things
// a bit simpler to bundle these together.
typedef struct { int x,y,z; } imu_xyz_t;

static inline imu_xyz_t
xyz_mk(int x, int y, int z) {
    return (imu_xyz_t){.x = x, .y = y, .z = z};
}

static inline void
xyz_print(const char *msg, imu_xyz_t xyz) {
    output("%s (x=%d,y=%d,z=%d)\n", msg, xyz.x,xyz.y,xyz.z);
}


// hard reset (reboot) of the device: should be the 
// same as a hard power cycle.  does not initialize anything.
// can be called for either accel or gyro.  any parameters
// set before won't be preserved.
void mpu6050_reset(uint8_t addr);


/************************************************************************
 * trivial accel interface: you may want to extend it w/ any i2c information
 * needed.
 */

// note: the names are short and overly generic: this saves typing, but 
// can cause problems later.
typedef struct {
    uint8_t addr;
    unsigned hz;    // samples / second
    unsigned g;     // sensitivity in g. 
} accel_t;


// passed to accel init.
enum { 
    accel_2g = 0b00,
    accel_4g = 0b01,
    accel_8g = 0b10,
    accel_16g = 0b11,
};

// initialize accel: scales readings to <accel_g>.
//
// likely want to extend so you can specify sample rate as
// well as which i2c is being used (for multiple devices).
accel_t mpu6050_accel_init(uint8_t addr, unsigned accel_g);

// blocking read of accel: returns raw (x,y,z) reading.
imu_xyz_t accel_rd(const accel_t *h);

// scale a reading returned by <accel_rd> to the sensitivity
// the device was initialized to.
imu_xyz_t accel_scale(accel_t *h, imu_xyz_t xyz);


/*******************************************************************
 * simple gyroscope interface.
 */

typedef struct {
    uint8_t addr; // device address. same as accel addr.
    unsigned hz;  // sample rate: likely != accel rate.

    // scale: for accel is in g, for gyro is in 
    // degrees per second (dps)
    unsigned dps;
} gyro_t;

enum {
    gyro_250dps  = 0b00,
    gyro_500dps  = 0b01,
    gyro_1000dps = 0b10,
    gyro_2000dps = 0b11,
};

// initialize gyroscope.  scaling set to the given degress
// per second.
gyro_t mpu6050_gyro_init(uint8_t addr, unsigned gyro_dps);

// raw reading of gyro.
imu_xyz_t gyro_rd(const gyro_t *h);

// scale reading by the initialized dps.
imu_xyz_t gyro_scale(gyro_t *h, imu_xyz_t xyz);


/*************************************************************************
 * used to talk to i2c: generally won't use these in the client.
 */

// set one i2c register <reg> to value <v> for device
// address <addr>:
//   <addr.reg> = <v>
void imu_wr(uint8_t addr, uint8_t reg, uint8_t v);

// read one i2c register <reg> from device 
// address <addr>.
uint8_t imu_rd(uint8_t addr, uint8_t reg);

// read <n> *contiguous* i2c registers for device <addr>
//   [<addr.base_reg>, <addr.base_reg>+n)
// into buffer <v>.  
// - size of <v> is at least <n> bytes.
// - does not check if the registers make sense.
int imu_rd_n(uint8_t addr, uint8_t base_reg, uint8_t *v, uint32_t n);
#endif
