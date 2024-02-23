## Equivalance checking finale.


<p align="center">
  <img src="images/pi-threads.jpg" width="700" />
</p>



By the end of the lab we will verify that the state switching assembly you wrote 
in the last lab is correct by:
 1. Dropping it into a pre-emptive threads package;
 2. Use your mini-step code from lab 10 to verify that the threads behave 
    identically under all possible thread interleavings.

Next week we will use this same basic approach to also verify that
virtual memory works.

Mechanically:
 1. You'll drop the assembly routines you wrote last time one at a time 
    replacing ours and check that the lab 10 tests still work.  You will
    be able to swap things in and out.

 2. Once that works run the threads and check hashes (more on this later).

----------------------------------------------------------------------
### Part 0: house-keeping

Today's lab requires invasive surgery on the code from your lab 10 (debug
hardware). Since that code already works, we don't want to mess with it
so we'll make a copy and crudely apply a patch.

Before doing anything else, make sure you have a clean working copy of
your debug code:

 1. Go to lab 10 `10-debug-hw/code` and make sure that `make check`
    still passes on all the tests!  (Make sure you're running all tests.)
 2. Copy that entire directory into today's lab `14-ss-equiv-deux/code`
 3. Then, again: make sure that `make check` passes in
    `14-ss-equiv-deux/code`, too.  We now know you have a clean, working
    initial state to start from.

Now apply the patch:
 1. Copy `14-ss-equiv-deux/code-patch` onto your `14-ss-equiv-deux/code` dir

        # from labs/14-ss-equiv-deux  
        % cp code-patch/* code 

 2. Make sure tests pass:

        % cd code
        % make check

Great you now have a working copy that you can modify!

What's new:
 - `switchto-asm.S` the routines you'll pull-in from last lab.
 - `full-except-asm.S`: the exception trampolines you'll pull-in from
    last lab.
 - `full-except.c` got some slight rewrites for system call exceptions.
 - `switchto.h` also got some rewrites to make it more regular.
 - `8-syscall-test.c`: a new system call test that shows how to setup
    a simple system call.  This weakly tests that your system call
    trampoline works correctly.

----------------------------------------------------------------------
### Part 1: start replacing routines and make sure tests pass.

The code for this part is setup so you can flip routines back and forth.
As is, everything should pass "out of the box"

Maybe the easiest way to start:
  1. Go through `switchto-asm.S`, in order, rewriting each routine to
     use the code you wrote in the last lab. (I would comment out the
     calls to our code so you can quickly put them back.) Make sure the
     tests pass for each one!  Once all are replaced, comment out our
     `staff-switchto-asm.o` from the Makefile.

     To help understand you should look at `switchto.h` which calls these 
     routines.

  2. Go to `full-except-asm.S` and write the state saving code.  This should
     mirror the system call saving you did last lab. 
     Make sure the tests pass for each one.  Once all calls to our code     
     are removed (I would comment them out so you can flip back if needed)
     remove the link from the Makefile.

     To help understand you should look at the `full-except.c` code which
     gets called from it.

Now do a "git pull" and hopefully the code for the crazy threads package
is checked in.
