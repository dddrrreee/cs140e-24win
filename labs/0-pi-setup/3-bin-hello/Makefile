# simple makefile that shows the tools and options used to compile 
# everything in this directory to make a 'hello world' program.
#  - shows how to define make variables and simple dependencies.

# the different tools we use.
ARM = arm-none-eabi
CC = $(ARM)-gcc
LD  = $(ARM)-ld
AS  = $(ARM)-as
OD  = $(ARM)-objdump
OCP = $(ARM)-objcopy
AS  = $(ARM)-as

# The gcc compiler flags
#    -Wall: warn about a bunch of suss things.
#    -Werror = must fix errors
#    -O: optimize (which allows for more -Wall error checking)
CFLAGS = -O -Wall -Wno-unused-variable -Werror \
          -nostdlib -nostartfiles -ffreestanding  -march=armv6 -std=gnu99  

all: hello.bin run

# simplest possible method: always recompile the whole thing.
#    1. generate a hello.elf with debugging symbols
#    2. disassemble hello.elf and put result in hello.list
#    3. strip binary and put result in hello.bin
hello.bin:  hello.c  libpi.a rpi.h ./Makefile
	$(CC) $(CFLAGS) staff-start.o hello.c -T memmap -o hello.elf libpi.a
	$(OD) -D  hello.elf > hello.list
	$(OCP) hello.elf -O binary hello.bin

run: hello.bin
	@echo "compile was successful: about to run <hello.bin>"
	pi-install hello.bin

clean:
	rm -rf *.bin *~ tags *.elf *.list *.d Makefile.bak 

# stop make from deleting these suffix'd files.
.PRECIOUS: %.list %.elf %.o
