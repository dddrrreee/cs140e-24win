NAME = load-state

# define this if you need to give the device for your pi
TTYUSB = 

OBJS := $(NAME).o asm.o

dep := $(OBJS:.o=.d) 

# VPATH := $(sort $(dir $(SRC)))

include $(CS140E_2022_PATH)/libpi/defs.mk
CFLAGS += -Wno-unused-function -Wno-unused-variable 

# add the memmap file, libpi and the makefile as dependencies
DEPS := $(MEMMAP) $(LPI) ./Makefile 

all: libs $(NAME).bin run


libs: 
	make -C $(LPP)

$(OBJS): $(DEPS)
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.S
	$(CC) -c $(CPP_ASFLAGS) $< -o $@
%.elf: $(OBJS) $(DEPS) 
	$(LD) $(START)  $(OBJS) -T $(MEMMAP) -o $@  $(LPI)


ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),cleanall)
-include $(dep)  # include dep files
endif
endif

./%.d: %.c $(DEPS)
	$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

%.list: %.elf
	$(OD) -D   $*.elf > $*.list
%.bin: %.list
	$(OCP) $*.elf -O binary $*.bin

run: $(NAME).bin
	my-install $(TTYUSB) $(NAME).bin

tags: 
	ctags *.[chSs] $(LPP)/*.[ch] $(LPP)/*.[chSs]

clean:
	rm -rf $(NAME) *~ tags *.o *.bin *.elf *.list *.img Makefile.bak  *.d 

.PHONY: libs all tags clean run

# stop make from deleting these suffix'd files.
.PRECIOUS: %.list %.elf %.o
