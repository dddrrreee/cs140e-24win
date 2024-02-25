## Equivalance checking finale.

<p align="center">
  <img src="images/pi-threads.jpg" width="700" />
</p>

***NOTE: There is now a complete working copy in `code-complete-staff`.  You
should be able to just "make check" etc here and copy your files over.***

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

----------------------------------------------------------------------
### Part 2: not much code but crazy: pre-emptive threads

For this part you have to copy from `code-patch-2`.

You will have to do some small changes to `mini-step.c`.

 1. Get rid of the `static inline` for:


        // change this from static inline in mini-step.c
        void mismatch_on(void);
        // change this from static inline in mini-step.c
        void mismatch_off(void);
        // change this from static inline in mini-step.c
        uint32_t mismatch_pc_set(uint32_t pc);

 2.  Add this new routine `mini-step.c`:

    void mismatch_run(regs_t *r) {
        uint32_t pc = r->regs[REGS_PC];

        mismatch_pc_set(pc);

        // otherwise there is a race condition if we are 
        // stepping through the uart code --- note: we could
        // just check the pc and the address range of
        // uart.o
        while(!uart_can_put8())
            ;

        switchto(r);
    }

  3. Your system call handler in `full-except-asm.S` will have to load the
     `INT_STACK` pointer into sp.

  4. Modify your makefile to compile `equiv-threads.c`.


  5. Useful debug:

```
// print the register diferrences
static void reg_dump(int tid, int cnt, regs_t *r) {
    uint32_t pc = r->regs[15];
    output("non-zero registers: tid=%d: pc=%x:", tid, pc);
    if(!cnt) {
        output("  {first instruction}\n");
    } else {
        int changes = 0;
        output("{ ");
        for(unsigned i = 0; i<17; i++) {
            if(r->regs[i]) {
                output(" r%d=%x, ", i, r->regs[i]);
                changes++;
            }
            if(changes && changes % 5 == 0)
                output("\n");
        }
        if(!changes)
            output("  {no changes}\n");
        else
            output("}\n");
    }
}
```



Now you should be able to run two tests:
  1. `9-equiv-test.c` this runs code without a stack.  The code is at
      the same place for all of us, so will give the same answer.

  2. `10-equiv-test.c`: this is more involved and prints output.

These should just run if your code is correct.

