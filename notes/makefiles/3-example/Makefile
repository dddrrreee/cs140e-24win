# show how to use wildcards to automate specifying everything
# that should get pulled into the makefile.
#
# have a single variable to define the program
# gather up all source and build and assume all
# source depends on all headers. 
#
# this means the body of the makefile is a generic template that 
# you can just repurpose.
# 
# note: the only thing you *would* need to change for a similarly
# constructed project would be <NAME>
NAME=main
CFLAGS = -O -Wall

# get all headers: will make everything depend on them.  this
# is a downside in general, but for small stuff we don't care
# (next few modifications change this)
hdr = $(wildcard *.h)
src = $(wildcard *.c)
# get all source
obj = $(src:.c=.o)

# below will work as long as you define <obj>, <hdr> and <NAME>
all: $(NAME) test

$(NAME): $(obj) $(hdr)
	$(CC) $(CFLAGS) $(obj) -o $(NAME)

%.o: %.c $(hdr)
	$(CC) $(CFLAGS) -c $<  -o $@

# no diff
test: $(NAME) 
	./$(NAME) > out
	diff out $(NAME).ref
	@echo "makefile: program '$(NAME)' passed test"
	@rm -f out

# note this does assume all trash you want to delete
# is in the current directory.
clean:
	rm -f $(NAME) out *~ *.bak $(obj)

.PHONY: clean test all
