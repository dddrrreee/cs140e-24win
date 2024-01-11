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

<p align="center">
  <img src="lab-memes/bug.jpg" width="400" />
</p>
