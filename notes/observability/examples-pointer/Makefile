# do examples 0 .. 10.  [as extension r/w this awful makefile!]
EX=0
# if you want to print debug output
SYM=sym.
SYM=
MK = make  --no-print-directory
CFLAGS = -O3 -g -fomit-frame-pointer   -mcpu=arm1176jzf-s # -mtune=arm1176jzf-s 
#CFLAGS += -ffreestanding
CC = arm-none-eabi-gcc 
#-Wall -O

all: $(EX)-pointer.o

$(EX)-pointer.o: $(EX)-pointer.c ./Makefile
	@echo "\n\n\n\n\n\n\n";
	@echo "-------------$(EX)-pointer.c ------------------\n";
	@cat $(EX)-pointer.c 
	@echo "------------- .o for $(EX)-pointer.c ---------------\n";
	@echo " arm-none-eabi-gcc $(CFLAGS) -c $(EX)-pointer.c";
	@arm-none-eabi-gcc $(CFLAGS) -c $(EX)-pointer.c
	@arm-none-eabi-objdump -d -S $(EX)-pointer.o > $(EX)-pointer.sym.dis
	@arm-none-eabi-objdump -d  $(EX)-pointer.o > $(EX)-pointer.dis
	cat $(EX)-pointer.$(SYM)dis
	@echo "---------------------------------------------------------------\n";
	@echo "\n\n\n\n\n\n\n\n";



# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)

# compile and generate dependency info;
# more complicated dependency computation, so all prereqs listed
# will also become command-less, prereq-less targets
#   sed:    strip the target (everything before colon)
#   sed:    remove any continuation backslashes
#   fmt -1: list words one per line
#   sed:    strip leading spaces
#   sed:    add trailing colons
%.o: %.c ./Makefile
	$(CC) -c $(CFLAGS) $*.c -o $*.o
	$(CC) -MM $(CFLAGS) $*.c > $*.d
	# $(CC) -MM $(CFLAGS) $*.c > $(notdir $*).d

	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

clean:
	rm -f  *.dis *.o *~ *.s *.d
