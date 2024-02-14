## Using an i2c mems-based IMU (accelerometer + gyroscope)

<p align="center">
  <img src="images/robot-pi.png" width="450" />
</p>


***Errata:***
- The given register values for the gyro are not correct
  (e.g., `CONFIG` and `GYRO_CONFIG`).  You should use the values in the
  PDF, not in the C code.  

- change the `bit_set` in `mpu6050_reset`:

        if(bit_set(imu_rd(addr, PWR_MGMT_1), 6))
            output("device booted up in sleep mode!\n");

- to `bit_is_on`:

        if(bit_is_on(imu_rd(addr, PWR_MGMT_1), 6))
            output("device booted up in sleep mode!\n");


Today we're going to communicate with the InvenSense MPU-6050
accelerometer + gyroscope using the I2C protocol.  Inertial measruement
units  (IMUs) are useful for measuring and controlling stuff based
on motion.  (E.g., gesture-based device control, step counting,  sports
movement tracking, movement reactive light shows, etc.)

Why:
 1. The code isn't complicated, and is a good break from debugging
    explosive execution stuff.
 2. Doing different devices is a good good basis for final projects.  
 3. The exact lab today can be used as a cheat code in other classes
    that have final projects --- e.g., just do what Tina did and 
    add an accel to your ai project (about 10 minutes) and immediately
    stand out from the other few hundred students.
 4. I2c is common, so it's good to get experience.  It's also 
    good to get practice with another device.

The plan is to:
  1. Write the 6050 device driver to configure and use the accelerometer
     using an i2c driver we give you.
  2. Write your own driver from the Broadcom documents.

  3. Extension: We also have several more advanced MPU's --- the
     InvenSenses MPU9250 and Polou's MiniMPU v5 --- both of which have
     a magnetometer.  In technical slang, these are called 9-axis DoF
     (degree of freedom) devices  --- 3-axis for the X, Y, Z accelerometer
     + 3-axis for the gyro + 3-axis for the magnetometer.  Porting your
     code to these is a good way to make it obvious what are the common
     things to do, and whats device-specific.

There are a ton of extensions.   Literally tons.  Many I wish I had
time to do, so with modest effort you can crank my boomer envy to 11.
(In my ideal world people go off and figure out how to do different
tricks and then do a show-and-tell next lab as a follow on.)

Hard (Daniel) mode:
 - You can easily ignore our starter code and write everything from
   scratch.  The needed interface is narrow (reset, initialize, has-data,
   read-data) without any datastructures.  It's an interesting exercise.


The `docs` directory has a bunch of documents.  The two main ones for
the MPU-6050:
  - [MPU-6050-reg-map.pdf](./docs/MPU-6050-reg-map.pdf): 
    The different device "registers" used to initialize and get readings.
  - [MPU-6050-spec.pdf](./docs/MPU-6050-spec.pdf): The device description that 
    gives a more general device overview along with self-test and startup delays.
  - [Broadcom-i2c.pdf](./docs/broadcom-i2c.pdf): excerpted broadcom I2c.


Some other documents in no particular order:
  - [A nice clear SPARKFUN tutorial on gyros](https://learn.sparkfun.com/tutorials/gyroscope/all).
  - [MPU6050 overview](https://mjwhite8119.github.io/Robots/mpu6050).

---------------------------------------------------------------------------
### Incomplete cheat sheet of page numbers.

The overall view of the device (`docs/MPU-6050-spec.pdf`): 
  - p7: 6050 only supports I2C (not SPI).
  - p10: overview of gyro and accel specs.  accel can eat a 10000g impulse ok.
  - p12/p13: useful distilled values for settings.
  - p12: self test difference from factory trim: -14% (min) to 14% (max).
  - p14: max voltage: 3.46v max (don't use 5v!).  2.375v min.
  - p14: "ramp rate": 100ms.  unclear if needed but we wait 100ms
    for device startup or reset.
  - p15: i2c addresses.  (0b1101000 or 0b1101001).
  - p15: i2c frequency (400khz fast mode, 100khz standard).
  - p27: has a self test.  configure it!
  
Note:
  - In general we can skip the external device reading (we
    don't have any connected) and the master/slave discussion.

For today, we want to figure out how to:
  1. Reset anything that we can back to a known state: the device, the
     FIFO (can have garbage), signal paths --- anything else that could
     have weird values.
  2. How long we need to wait after reseting / powering up.
  3. Enable the accelerometer and gyroscope and set their datarate and 
     sensitivity.
  4. Know that data is available.

The register document (`docs/MPU-6050-reg-map.pdf`): what register to
read/write with what values:

  - p6-8: entire register map.
  - p8: reset values = 0 for everything but 
        power management (reg=107, val=0x40) and `WHO_AM_I` (reg=117, val=0x68)
  - p9: "the device will come up in sleep mode upon power up"
  - p9-12: self-test.  for many devices can't be sure the device is working,
    because we don't know what the right answer is.  so part of the data
    sheet adventure is searching around for different known answers and
    cross checking.  self-test values give us a way to make sure both that
    our code is correct and that the device isn't junk.  
  - p14: gyro configuration (reg= 0x1b or 27).   controls self-test and 
    the gyro sensitivity (degrees per second).
  - p15: accel configuration 1 (reg 28 or 0x1c) self test
        and accel sensitivity (2g min up to 16g max).
  - p16: FIFO enable (reg=35 or 0x23).
  - p19: i2c clock (the reset value 0 seems to = 348khz?)   possibly
    this is not relevant.
  - p28: interrupt enable and p29: interrupt status.
    I haven't used these but they are good because you can make sure
    you don't read stale data and don't miss anything.
  - p30-p31: accel readings (reg 59-64): you get a high byte and a low byte.
    make sure you combine these correctly!
  - p31: temperature (65 and 66).

  - p32: gyro output (regs=67-72, 0x43-0x48).  sensitivity set by `FS_SEL`
    (reg 27).
  - p38: reset signal path.  does not reset the sensor registers.  i think
    worth reseting during power up, but i think can do with "user control"
    (p40) below.
  - p38: motion control.  the DPM is proprietary so you'll have to look through
    git hub for how to use.
  - p40: "user control" that gives a bunch of things that can be reset:
    enable/disable/reset FIFO, signal reset.
  - p41: power managmement 1 (reg 107, 0x6b): use to reset the device, 
    configure stuff.  in theory: after reset starts in "sleep mode" so
    you have to turn that off!  difference from 6500.

  - p42: power management 2 (reg 108): used in low-power mode (which we don't).
  - p44: FIFO count registers (regs 0x72,0x73).
  - p45: read/write fifo (0x74): these are written in order of register 
    number from lowest to highest.  must check count before reading.
    (note: if devices produce values at different rates i'm not sure how
    to disambiguate the contents).  i've seen recommendations we should
    use the fifo to make sure we get a clean internally-consistent read
    of registers rather than some old and some new (if a reading comes
    in while extracting them).

  - p46: `WHO_AM_I` (117): use this to validate you can read from the
    chip and its responsive.

---------------------------------------------------------------------------
### 1: fill in the accelerometer code in the code directory.

<p float="center">
  <img src="images/6050-top.jpg" width="300" />
  <img src="images/6050-side.jpg" width="300" />
</p>

What to do:
 1. Plug in your mpu-6050 and make sure that the staff code works.
    It should give *roughly* 1000mg or -1000mg readings for the different
    axis that are pointed to the ceiling.
 2. Look in the driver code `driver-accel.c` to see how it's calling
    the accel code.
 3. Look at the interface description in `mpu-6050.h`.
 4. Write the accel routines in `mpu-6050.c`.
 5. Make sure that the results make sort-of sense.

Use the datasheet and application note from the docs directory.
There are notes in the code.

Note:
  - We use the "data ready" interrupt to see when new data is available.
    With the checked-in i2c implementation this check will *never* fail 
    because the speed is too slow.   I checked in a faster staff i2c
    (or you can write your own!) to fix this.  Interesting bug to 
    track down since it's also consistent with misconfiguration.


---------------------------------------------------------------------------
### 2: fill in the gyroscope code in the code directory.

Similar to accel:
 1. Look at the readings from our code.
 2. Look at `driver-gyro.c`
 3. Look at the interface in `mpu-6050.h`.
 4. Write the gyro routines in `mpu-6050.c`.

Use the datasheet and application note from the prelab.  Start with
the simple cookbook example they give and make sure your stuff looks
reasonable!

---------------------------------------------------------------------------
### Extension: display the readings using your light strip or LED

The nice thing about the light strip is that you can do high-dimensional displays easily.
One dumb way:
   - give the accel half and the gyro half (or do not!  just map their coordinate system
     to the entire thing).
   - map the 3-d point of each reading to a location.
   - map the velocity of the point (or the accel, no pun) to a color.
   - display!

---------------------------------------------------------------------------
### Extension: write your own bit-banged i2c

This is fun, and not hard from the wikipedia.

---------------------------------------------------------------------------
### Extension: multiple devices + i2c

If you have your own i2c, you can easily hook up more than one device.
This is a good step towards a sensor glove or a wearable harness.

---------------------------------------------------------------------------
### Some Legit Extensions

If you finish, there's lots of tricks to play.  A major one is doing
correcton to the device errors.  The accel / gyro device is not that
accurate without correction.  This article discusses the issues and what
can be done (simply):

  - [Correct for hard-ion](https://www.fierceelectronics.com/components/compensating-for-tilt-hard-iron-and-soft-iron-effects)

Different writeups in `./docs` for different directions:
   1. [Make a legit compass](./docs/AN203_Compass_Heading_Using_Magnetometers.pdf)
   2. [Calibrate](./docs/AN4246.pdf)
   3. [Even more compass + Calibrate](./docs/AN4248.pdf)
   4. [Location](./docs/madgewick-estimate.pdf)


