BOOTLOADER = my-install

PROGS += tests/1-test-basic.c
PROGS += tests/1-test-setup.c
PROGS += tests/1-test-two-addr.c
PROGS += tests/2-test-procmap.c

O = $(CS140E_2024_PATH)/libpi
STAFF_OBJS += $(O)/staff-objs/staff-full-except.o
STAFF_OBJS += $(O)/staff-objs/staff-full-except-asm.o
STAFF_OBJS += $(O)/staff-objs/staff-switchto-asm.o

# this is the one file you'll modify.
COMMON_SRC += pt-vm.c

# you'll write this next time.
COMMON_SRC += your-mmu-asm.S 

# we give you this.
COMMON_SRC += mmu-helpers.c
COMMON_SRC += mmu.c

# make sure can drop in staff.
STAFF_OBJS += staff-mmu-asm.o  
STAFF_OBJS += staff-mmu.o  
STAFF_OBJS += staff-pt-vm.o  

#COMMON_SRC += staff-mmu-asm.S 
#COMMON_SRC += staff-mmu-except.c 
#COMMON_SRC += staff-mmu.c 
#COMMON_SRC += staff-pt-vm.c

# define this if you need to give the device for your pi
TTYUSB = 

# set RUN = 1 if you want the code to automatically run after building.
RUN = 1

include $(CS140E_2024_PATH)/libpi/mk/Makefile.robust

clean::
	make -C tests clean
