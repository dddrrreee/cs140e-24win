# add a pattern matching rule for .o's  instead of manually specifying.
#
# the safety checking actually makes a bit longer than our previous
# Makefile :)  however, this approach works much better in practice
# since effort doesn't grow with the number of files.
CFLAGS = -O -Wall

all: main test

main: main.o a.o b.o c.o
	$(CC) a.o b.o c.o main.o -o main

# disable built-in matching.  good form but not necessary.
%.o:%.c

# pattern matching rule that generates a .o from a .c in the same
# location.
#
# we print out what we are doing (using a comment, which is a bit weird!)
# just so we can see it --- there's no need for this in general.
#
# as you can see from the print:
#   - $< is the input .c file name
#   - $@ is the output .o file name.
%.o: %.c header.h
	# doing %.o=$@ [this is $$@] and and %.c=$< [this is $$<]
	$(CC) $(CFLAGS) -c $<  -o $@

# add this extra rule to detect if we missed a .o 
%.o:
	$(error "should never run this: some .o not handled: <$@>")

# same as before
test: main
	./main > out
	diff out main.ref
	@echo "makefile: program 'main' passed test"
	@rm -f out

# same as before
clean:
	rm -f main out *~ *.bak *.o *.d

.PHONY: clean test
