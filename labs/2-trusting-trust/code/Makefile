# change these so you can do a "make test" and it verifies what you already
# have in there.  (probably need a pi in there).   also you will need to 
# do a better design.
# ah: for test, doesn't work for us b/c we don't have the code...
#
# also it is tricky if they swap with our code and their code.
SUBDIRS= step1 step2 step3

.PHONY: all check clean
all check clean: $(SUBDIRS)

all: TARGET=all
check: TARGET=check
clean: TARGET=clean

submit:
	zip upload.zip step1/*.c step2/*.c step3/*.c

# No, you can't do TARGET=$@, or at least I don't know how to.

# recursive call to make
$(SUBDIRS): force
	$(MAKE) -s -C $@ $(TARGET)

clean:
	rm -f *~ *.bak

.PHONY: force
	force :;
