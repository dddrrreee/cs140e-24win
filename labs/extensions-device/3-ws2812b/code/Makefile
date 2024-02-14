# do this last.
PROGS = 2-neopix.c 
# do this second:
PROGS = 1-blink.c  
# do this first.
PROGS = 0-timing-check.c 

# your source, shared by driver code.
#   if you want to use our staff-hc-sr04.o,
#   comment SRC out and uncomment STAFF_OBJS
COMMON_SRC := neopixel.c

OPT_LEVEL = -O3

RUN = 1

DEPS = ./Makefile

include $(CS140E_2024_PATH)/libpi/mk/Makefile.template-fixed
