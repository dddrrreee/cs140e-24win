COMMON_SRC += mini-watch.c
COMMON_SRC += mini-step.c
# this changed
COMMON_SRC += staff-full-except.c 
# new
COMMON_SRC += switchto-asm.S
# new
COMMON_SRC += full-except-asm.S

# you'll be replacing these.
STAFF_OBJS := staff-full-except-asm.o  
STAFF_OBJS += staff-switchto-asm.o

# define this if you need to give the device for your pi
TTYUSB = 
BOOTLOADER = my-install

# set to 0 if you don't want it to run
RUN=1

START_ASM = staff-start.S

# new test 8
PROGS := $(wildcard ./[125678]-*.c)

GREP_STR := 'TRACE:\|THREAD:\|ERROR:\|PANIC:' 
include $(CS140E_2024_PATH)/libpi/mk/Makefile.template-fixed
