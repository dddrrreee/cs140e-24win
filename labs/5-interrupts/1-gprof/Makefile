# need the makefile to give an error if this doesn't exist
PROGS := gprof.c

# use our own private copy so can modify it.
COMMON_SRC += interrupts-asm.S

# the rest of the code we take from <0-timer-int>
COMMON_SRC += ../0-timer-int/interrupts-c.c	
COMMON_SRC += ../0-timer-int/timer-interrupt.c 

# for kmalloc (you'll build this in a couple labs)
STAFF_OBJS = staff-kmalloc.o

# uncomment if you want it to automatically run.
# RUN=1

# define this if you need to give the device for your pi
TTYUSB = 
BOOTLOADER = pi-install

include $(CS140E_2024_PATH)/libpi/mk/Makefile.template-v4
# include $(CS140E_2024_PATH)/libpi/mk/Makefile.simple
