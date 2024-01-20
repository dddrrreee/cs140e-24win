first:
    - fix all the test cases
    - add a "make checkoff"   
        get rid of emit.

    - is there a way to make the tests better?

    - more precise about the register tests?
    - have the write register tests?

+ check that the sp is in the stack.

what to add:
    - rpi_yield and show it speeds up UART
    - debug malloc for corruption.
    - instrument the routines for stack corruption.



    - signal/wait in general.
    - is there some way to do the signal / wait in a way that works.

    - is there some way to do the interrupt interlacing?
        where you check that its in the thread package using the pc

    - have real-time?  could add this.

fix the pulldown, pull up
    what do we have to the writeup for this?  how do you connect the LED?


OH:
    can we use the tests seperately and just compile with our own code.staff?

------------------------------------------------------------------------------------
ok: cool.  nice easy example:
    drop in threads
    have the uart speed up in terms of total time b/c of interleaving.

    - give them a program with normal stuff and with the printk.
    - run with and without threads.

    - have a "is thread started

- see how fast it runs.
- also have a redzone for stack corruption.
    - you really want to grab on each procedure call.  can you do that with
    gcc?


can have them do a generic thread create?  drop it into any routine,
it creates a thread, context switches so it's transparent 

    the issue here is that you need to grab the stack variables and such from the
    current stack.

    - how do you get the initial stack pointer value?  will backtrace give it?

        is there some way to rewrite the current callsite?


need to make the timer64 bit routine --- we were stupidly not using the high 
bits.   how would you set the deadline?  actually, we could use this for 
pre-emptive easily?  just see that it's in the code and don't do if it is.

  void __cyg_profile_func_enter (void *this_fn, void *call_site);
  void __cyg_profile_func_exit  (void *this_fn, void *call_site);

i like the not-interrupt in the threads package using the code.  this is 
a cute idea.
    - or we could just do pre-emptive and do that?

    - is there a way to verify without having ss?

very cool.
    - have them rewrite the code so it's a single clean file?  [or give them 
      this?]

you want a solid place to stand.  if you understand the interrupt lab, you
have a solid floor, and can walk to places that are steps away.  if you're
wobbling and about to fall any step can easily be a faceplant.
