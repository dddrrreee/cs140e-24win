SUBDIRS= 01-tracing-pi  03-fake-pi	 1-fake-pi 2-trace check-hello check-libpi

# this only works on linux
SUBDIRS += 02-tracing-linux  

.PHONY: all check clean
all check clean: $(SUBDIRS)

all: TARGET=all
check: TARGET=check
clean: TARGET=clean

# No, you can't do TARGET=$@, or at least I don't know how to.

# recursive call to make
$(SUBDIRS): force
	$(MAKE) -s -C $@ $(TARGET)

clean:
	rm -f *~ *.bak

.PHONY: force
	force :;
