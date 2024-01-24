# need the makefile to give an error if this doesn't exist
PROGS := 0-syscall.c 1-syscall.c
COMMON_SRC := syscall-asm.S  interrupts-asm.S

# we use the libpi interrupts-c.c

# uncomment if you want it to automatically run.
# RUN=1

# define this if you need to give the device for your pi
TTYUSB = 
BOOTLOADER = pi-install

include $(CS140E_2024_PATH)/libpi/mk/Makefile.template-v4
