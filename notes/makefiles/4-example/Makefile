# real problem with previous makefile:
#  1. had to specify dependencies manually.
#  2. makes it hard to use wildcard rules accurately: either 
#     you specify everything depends on everything or you
#     have a bunch of rules, defeating the purpose.
# 
# instead we use gcc to automatically generate dependencies.
# 
NAME=main

# standard dependency flags so gcc will generate the includes
# that a file depends on.  "$*.d" places the .d file in the same
# location as the .c
# from 'make' maintainer:
#   https://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d
CFLAGS = -O -Wall $(DEPFLAGS)

# in general use the ":=" assignment b/c it will evaluate
# the right hand side (RHS) once, at the assignment.
src := $(wildcard *.c)
obj := $(src:.c=.o)
# .d files for dependencies
dep := $(src:.c=.d)

all: $(NAME) test

$(NAME): $(obj)
	$(CC) $(CFLAGS) $(obj) -o $(NAME)

# note: we add a .d as a dependency to catch if (1)
# it got deleted or (2) was put in a different location.
# easy to forget this and be completely unprotected.
%.o: %.c  %.d
	$(CC) $(CFLAGS) -c $<  -o $@

# include the generated dependency rules.
# open up a .d file in your editor to see 
# why this works!
# 
# put it last so the rules can't mess with ours.
-include $(dep)

# tell gcc not to worry if a .d doesn't exist since it gets
# made during compilation. 
$(dep): %.d: ;

# print out the different variables.
print:
	# src = <$(src)>
	# obj = <$(obj)>
	# dep = <$(dep)>
	# DEPFLAGS = <$(DEPFLAGS)>
	
test: $(NAME) 
	./$(NAME) > out
	diff out $(NAME).ref
	@echo "makefile: program '$(NAME)' passed test"
	@rm -f out

clean:
	rm -f $(NAME) out *~ *.bak $(obj) $(dep)

.PHONY: clean test all print
