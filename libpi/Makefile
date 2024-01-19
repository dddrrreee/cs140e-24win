# simple definitions for making libpi.a
#  - you should only ever have to modify SRC or STAFF_OBJS

# to add your gpio.c:
#   1. copy gpio.c to libpi/src/gpio.c
#   2. swap the two lines below.
STAFF_OBJS  +=  ./staff-objs/gpio.o
# SRC += src/gpio.c

# you would add the path to yours.
STAFF_OBJS  +=  ./staff-objs/uart.o
STAFF_OBJS  +=  ./staff-objs/gpio-pud.o
STAFF_OBJS  +=  ./staff-objs/gpio-int.o 

# you should not have to modify this variable.  
SRC :=  $(SRC)                              \
        $(wildcard ./libc/*.[Sc])           \
        $(wildcard ./staff-src/*.[Sc])      \
        staff-start.S

DEPS += ./Makefile ./manifest.mk

# hack to minimize git conflicts: we do various customizations
# in there; but probably would be clearer to inline it.
include ./manifest.mk
