What to do:

   1. `00-hello`: compile and make sure this code runs on your pi.
      look through the .c files to see how to use `demand` `assert` etc.

   2. `01-tracing-pi`: run and look through this code.  it's a
      simple example of how to override routines using the linker.
      You will use this to trace the PUT/GET calls when run on the
      actual pi hardware and do the same comparison to everyone else,
      and compare to when you replace our staff `gpio.o` with your own.

   3. `02-tracing-linux` if you're on linux you can also run similar code
      for step 2.  AFAIK MacOS's linker  does not support this option.

   4. `03-fake-pi`: this shows increasingly fancy examples of taking
      r/pi code and tricking it into running on your laptop (i.e.g.,
      not the pi hardare) by making fake versions of different r/pi
      library routines.

      It is setup to run both on the pi and your laptop.   You should
      look through the subdirectories (`0-libpi-fake` `1-libpi-fake`
      `2-libpi-fake`) and try running them by changing the
      `03-fake-pi/Makefile` to use the different version.

   5. `code-fake-pi` : a more full fledged but still fairly
      trivial user-level emulator for your pi code.  You will use this
      approach to cross-check your implementation against everyone else's
      to ensure everyone is equivalant.  Thus, if one person is correct,
      everyone is correct.


If you're going to modify these, please copy the directory --- I'm
still modifying and playing around with the code, so you'd get conflicts
otherwise.

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
