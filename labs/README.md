## The labs

<p align="center">
  <img src="lab-memes/coding.jpg" width="400" />
</p>

Below describes where we're going and why.   The labs themselves have a
lot more prose on each topic.  There is a pointer to where we currently
are: the labs after this point can still see significant revisions.

  - Labs usually have some [crash-course notes](../notes/README.md) about
    key concepts --- you must read these before the lab or you'll be lost.
    We also suggest re-reading some number of labs later after the concepts
    have sat for a bit so you get a deeper view.  Note summary (so far):

A note on lab structure:

  - The labs are deliberately *not* organized as a
    self-contained set of blocks --- we don't do all the labs for topic A,
    then all the labs for topic B, etc.  Instead, we break up each topic
    into pieces and do some of A, then some of B, then circle back to A,
    then circle back to B, etc.

  - This approach is based on newer methods from learning
    theory ("spaced repetition") that (hopefully) make the topics sink
    in deeper.  They are also used to break up hard topics with easier
    (but not easy) "breather" labs to give more time to get stuff under
    control and consolidate.

---------------------------------------------------------------------
### 0: non-pi hacking

Unlike all subsequent labs, our first two don't use hardware.  They should
give a good feel for whether the class works for you without requiring
a hardware investment.

  - [0-intro](0-intro): the intro (non-lab) lecture.

  - [1-compile](1-compile): This short lab focuses on what happens when
    you compile code.  How to automate compilation with `make`.  How to
    see how the compiler translated your C code by examining the machine
    code it produced.  And some some of the subtle rules for how the
    compiler can do this translation.  What we cover will apply to every
    lab this quarter.

  - [2-trusting-trust](2-trusting-trust): Ken Thompson is arguably our patron
    saint of operating systems --- brilliant, with a gift for simple code that
    did powerful things.   We will reimplement a simplified version of a crazy
    hack he described in his Turing award lecture that let him log into any
    Unix system in a way hidden even from careful code inspection.

---------------------------------------------------------------------
### 1: Going down to metal (part I)

The first few labs will writing the low-level code needed to run the
r/pi and using modern techniques to validate it.  Doing so will remove
magic from what is going on since all of the interesting code on both
the pi and Unix side will be written by you:


  - [3-gpio](3-gpio/):  Two parts.  First, we will give out the
    hardware and make sure it works: [0-pi-setup](0-pi-setup/README.md).

    Second start getting used to understanding hardware datasheets by
    writing your own code to control the r/pi `GPIO` pins using the
    Broadcom document GPIO description.  You will use this to implement
    your own blink and a simple network between your r/pi's.

    ***READING***:
       - Note: [GPIO](../notes/devices/GPIO.md).
       - Note: [crash course in writing device code](../notes/devices/DEVICES.md).
       - pages 4--7 and 91---96 of the broadcom
         datasheet (`docs/BCM2835-ARM-Peripherals.annot.PDF`)

  - [4-cross-check](4-cross-checking): you will use read-write logging
    of all loads and stores to device memory to verify that your GPIO
    code is equivalent to everyone else's.  If one person got the code
    right, everyone will have it right.

    A key part of this class is having you write all the low-level,
    fundamental code your OS will need.  The good thing about this
    approach is that there is no magic.  A bad thing is that a single
    mistake can make a miserable quarter.  Thus, we show you modern
    (or new) tricks for checking code correctness.


---------------------------------------------------------------------

<p align="center">
  <img src="lab-memes/bug.jpg" width="400" />
</p>
