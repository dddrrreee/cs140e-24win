# useful to see different features of architecture
# PROGS := 0-align-test.c 0-delay-test.c

# run all three tests 
PROGS := 1-test-wait-ncycles.c
# PROGS += 2-test-bitbang-put8.c
# PROGS += 3-test-sw-uart-hello.c

# checkoff.
PROGS := $(wildcard [123]*-test*.c)

# all of these should be turned off for checkoff.  
# to use your sw-uart uncomment:
# COMMON_SRC :=  sw-uart.c
# to use ours:
# STAFF_OBJS := staff-sw-uart.o

# RUN=0
OPT_LEVEL = -O3

# swap these if your bootloader doesn't work.
BOOTLOADER = my-install
BOOTLOADER = pi-install

include $(CS140E_2024_PATH)/libpi/mk/Makefile.template-fixed

