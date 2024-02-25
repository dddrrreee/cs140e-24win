LPP := $(CS140E_2024_PATH)/libpi/

# Put yours in, along with armv6-debug-impl.h and remove the staff.o
# COMMON_SRC += mini-watch.c
# COMMON_SRC += mini-step.c

COMMON_SRC += full-except-asm.S
COMMON_SRC += switchto-asm.S
COMMON_SRC += equiv-threads.c
COMMON_SRC += staff-full-except.c 

START_ASM = staff-start.S

STAFF_OBJS += staff-mini-step.o
STAFF_OBJS += staff-mini-watch.o
STAFF_OBJS += staff-full-except-asm.o
STAFF_OBJS += staff-switchto-asm.o

# tests.
PROGS := $(wildcard ./[125678]-*.c)
# these do not have .out's --- should be self-checking.
# PROGS = 9-equiv-test.c
# PROGS += 10-equiv-test.c

TTYUSB = 
BOOTLOADER = my-install
RUN=1

GREP_STR := 'TRACE:\|THREAD:\|ERROR:\|PANIC:' 
include $(CS140E_2024_PATH)/libpi/mk/Makefile.template-fixed
