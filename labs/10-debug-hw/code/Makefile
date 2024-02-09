# you'll implement <mini-watch.c> later in lab
COMMON_SRC += mini-watch.c
COMMON_SRC += staff-full-except.c 

# you'll do these next week.
STAFF_OBJS := staff-full-except-asm.o  staff-switchto-asm.o

# define this if you need to give the device for your pi
TTYUSB = 
BOOTLOADER = my-install

# set to 0 if you don't want it to run
RUN=1

START_ASM = staff-start.S

# simple watchpoint test
PROGS := 1-watchpt-test.c  2-brkpt-test.c  

# PROGS := $(wildcard ./[123456]-*.c)

GREP_STR := 'TRACE:\|THREAD:\|ERROR:\|PANIC:' 
include $(CS140E_2024_PATH)/libpi/mk/Makefile.template-fixed
