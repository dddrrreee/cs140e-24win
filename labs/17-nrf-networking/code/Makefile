# easiest
# PROGS := 0-no-ack-check.c
# then.
# PROGS := 0-ack-check.c

# ordered easy to harder
PROGS += $(wildcard tests/0-*.c)
PROGS += $(wildcard tests/1-*.c)
PROGS += $(wildcard tests/2-*.c)
PROGS += $(wildcard tests/3-*.c)

# initially everything should pass.
# PROGS := $(wildcard tests/[0123]-*.c)

COMMON_SRC := nrf-hw-support.c nrf-public.c nrf-driver.c

STAFF_OBJS += $(CS140E_2024_PATH)/libpi/staff-objs/new-spi.o
STAFF_OBJS += ./staff-nrf-driver.o

# uncomment if you want it to automatically run.
RUN=1

LIBS += $(CS140E_2024_PATH)/lib/libgcc.a

BOOTLOADER = my-install
EXCLUDE ?= grep -v simple_boot
include $(CS140E_2024_PATH)/libpi/mk/Makefile.robust

clean::
	make -C tests clean
