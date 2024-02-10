# similar to 0-makefile but does incremental compilation by manually
# hardcoding the names and rules for .o files.
#
# the next makefiles will do fancier versions.

CFLAGS = -O -Wall

all: main test

main: main.o a.o b.o c.o 
	$(CC) a.o b.o c.o main.o -o main

# compile the .o's individually.
#
# we manually specify each target (a.o, b.o, etc) as
# well as their dependencies. 
a.o: a.c header.h
	$(CC) $(CFLAGS) -c a.c -o a.o
b.o: b.c header.h
	$(CC) $(CFLAGS) -c b.c -o b.o
c.o: c.c header.h
	$(CC) $(CFLAGS) -c c.c -o c.o
main.o: main.c header.h
	$(CC) $(CFLAGS) -c main.c -o main.o

##################################################################
# below is the same as 0-makefile

# the `@` in front of the line causes make to not print the command 
test: main
	# test: rule
	./main > out
	diff out main.ref
	@echo "makefile: program 'main' passed test"
	@rm -f out

# cleanup remove outputs and temporary files
clean:
	rm -f main out *~ *.bak *.o *.d

.PHONY: clean test all
