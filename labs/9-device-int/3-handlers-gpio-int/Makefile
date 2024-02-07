# to use your gpio-int.c swap these.
# COMMON_SRC := ../2-gpio-int/gpio-int.c
STAFF_OBJS += $(CS140E_2024_PATH)/libpi/staff-objs/gpio-int.o

#STAFF_OBJS += $(CS140E_2024_PATH)/libpi/staff-objs/interrupts-asm.o
#STAFF_OBJS += $(CS140E_2024_PATH)/libpi/staff-objs/interrupts-vec-asm.o

# uncomment this if our libpi
# LPI = ../staff-objs/libpi.a

# level 3: do N iterations of event type.
# easiest: one type of event.
PROGS = 3-N-rising-edge.c
# harder: rise + fall
PROGS = 3-N-rise-fall.c
# hardest: everything.
PROGS = 3-N-rise-fall-timer.c

# level 2: combine more than one event type
# easier
PROGS = 2-rise-fall-edge.c
# has timers so is harder.
PROGS = 2-timer-rise-fall-edge.c

# level 1: easiest: do a single event once 
# (other than for timer)
PROGS = 1-falling-edge.c
PROGS = 1-rising-edge.c
PROGS = 1-timer.c

# everything.
PROGS = $(wildcard [123]-*.c)

COMMON_SRC += test-interrupts.c
COMMON_SRC += interrupt-vec.c interrupt-asm.S 

# uncomment if you want it to automatically run.
# RUN=1

CAN_EMIT=0
EXCLUDE ?= grep -v simple_boot
# include $(CS140E_2024_PATH)/libpi/mk/Makefile.template-fixed
include $(CS140E_2024_PATH)/libpi/mk/Makefile.robust

# do a checkoff.
checkoff:
	make -f checkoff.mk
