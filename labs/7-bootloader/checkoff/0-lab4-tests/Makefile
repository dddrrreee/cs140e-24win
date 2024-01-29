TESTS = $(wildcard ./*.bin)

# you should not need this.
# TTYUSB = 

# ugh: my-install does a BOOT:, we do TRACE:
BOOTLOADER = my-install
BOOTLOADER = pi-install

# we only look at this output

GREP_STR := "TRACE:"
GREP_STR := 'PI:\|TRACE:\|ERROR:\|PANIC:\|simple_boot'
EXCLUDE := cat

include $(CS140E_2024_PATH)/libpi/mk/Makefile.check-v3
