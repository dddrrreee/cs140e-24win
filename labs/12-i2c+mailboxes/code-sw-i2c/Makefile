# sort of gross:
#   1. make a sym link to your mpu6050;
#   2. or just copy it over.
#   3. or put your mpu6050.c and mpu6050.h in libpi/src and the
#      driver here.

$(error "copy/symlink your MPU6050 in a subdir here (or put in libpi) and update the makefile!");

# if you put it in a subdir.
IMU_DIR = mpu6050/

# this is just from last lab.
PROGS = $(IMU_DIR)/driver-accel.c

COMMON_SRC = sw-i2c.c sw-i2c-vaneer.c 

# update the location.
COMMON_SRC += $(IMU_DIR)/mpu-6050.c

# you can swap your hw i2c back and forth or the staff .o
# COMMON_SRC = i2c.c


# define this if you need to give the device for your pi
TTYUSB = 
BOOTLOADER = my-install

# uncomment if you want it to automatically run.
RUN=1

LIBS += $(LGCC)
LIB_POST := $(LGCC)

include $(CS140E_2024_PATH)/libpi/mk/Makefile.template
