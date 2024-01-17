### Prelab for fake-pi

Reading:
  1. The pullup/pulldown discussion in the broadcom document (see more
     discussion below).  You're going to implement this.
  2. Read [03-fake-pi/README.md](03-fake-pi/README.md) to see simple
     examples of emulating pi code on Unix.
  3. Read the [1-fake-pi README](1-fake-pi/README.md) a few
     times and look through the code.  Understanding how to run r/pi
     code unaltered on a fake pi Unix emulator is a crucial concept that
     we'll use throughout the quarter.

What to do:

   1. `01-tracing-pi`: run and look through this code.  it's a
      simple example of how to override routines using the linker.
      You will use this to trace the PUT/GET calls when run on the
      actual pi hardware and do the same comparison to everyone else,
      and compare to when you replace our staff `gpio.o` with your own.

   2. `02-tracing-linux` if you're on linux you can also run similar code
      for step 2.  AFAIK MacOS's linker  does not support this option.

   3. `03-fake-pi`: this shows increasingly fancy examples of taking
      r/pi code and tricking it into running on your laptop (i.e.g.,
      not the pi hardare) by making fake versions of different r/pi
      library routines.

      It is setup to run both on the pi and your laptop.   You should
      look through the subdirectories (`0-libpi-fake` `1-libpi-fake`
      `2-libpi-fake`) and try running them by changing the
      `03-fake-pi/Makefile` to use the different version.

   4. `1-fake-pi` : a more full fledged but still fairly
      trivial user-level emulator for your pi code.  You will use this
      approach to cross-check your implementation against everyone else's
      to ensure everyone is equivalant.  Thus, if one person is correct,
      everyone is correct.

To pop up a few levels, the raw tracing of PUT/GET enables some
interesting tricks:

  1. Check fidelity of your fake-pi emulator:  You can store the PUT/GET
     calls emitted from a raw run to a file (a log) and then change the
     fake pi so it optionally reads from this log: when you replay them
     the result should be identical.  (Including when you log the PUT/GET
     done during the replay, emit that to a log, replay that, etc.)

  2. Partial evaluation: You can also write a program that reads this
     log and emits a pi program that executes just these exact calls ---
     you can use this trick to replace the original program them with this
     (often much simpler) low-level sequence of PUTs and GETs.
