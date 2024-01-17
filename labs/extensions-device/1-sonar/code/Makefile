# checkoff: comment out STAFF_OBJS
#
# this is the makefile template we use --- a bunch of stuff
# happens in the included makefile.  we will simplify, but
# for the moment, time-pressure.

# the driver programs: these all must have a notmain.
PROGS := 1-sonar.c
COMMON_SRC := hc-sr04.c 

# to make sure you only use your code, comment 
# this out with a '#'
STAFF_OBJS = staff-hc-sr04.o 

# define this if you need to give the device for your pi
TTYUSB = 
BOOTLOADER = pi-install

# uncomment if you want it to automatically run.
RUN=1

include $(CS140E_2024_PATH)/libpi/mk/Makefile.template
