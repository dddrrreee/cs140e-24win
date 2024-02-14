PROGS = driver-accel.c
# PROGS = driver-gyro.c

# when you build yours swap this.
# COMMON_SRC = mpu-6050.c
STAFF_OBJS += staff-mpu-6050.o

# if you make your own i2c, get rid of this.
# default: is too slow.
# STAFF_OBJS += staff-i2c.o 
STAFF_OBJS += staff-faster-i2c.o 

# set to 1 if you want it to automatically run.
RUN=0

# define this if you need to give the device for your pi
TTYUSB = 
BOOTLOADER = my-install

LIBS += $(LGCC)
LIB_POST := $(LGCC)
include $(CS140E_2024_PATH)/libpi/mk/Makefile.template-fixed
