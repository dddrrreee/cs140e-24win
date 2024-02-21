## Single step equivalance checking



<p align="center">
  <img src="images/pi-ss-equiv.jpg" width="700" />
</p>


***NOTE***:
  - You can set just the `cpsr` mode using a register e.g., r0 via the
    `msr` assembly instruction with the `_c` modifier:

            msr cpsr_c, r0
 
    You can see an example of where we use `msr` in `libpi/staff-start.S`

  - `prefetch_flush(r)` expects a register `r` it can trash.  Usually you
     want a caller-saved that isn't in use.***

  - For part 0: you can assume `mode_get_lr_sp_asm` is *not* called
    for USER mode.  We'll have a seperate routine for that.  

Initially we were going to do today's lab in one shot, but we'll split
it across two labs so that:
  1. People can check off from last week since so many people missed.
  2. You can meet and start doing your project: you should get your
     project approved today and ideally put in at least a couple hours
     work getting parts and coordinating.

The downside is that today is more of a pre-lab since the cool final
result will happen thursday.


The entire lab (part1 + part 2) will be building a wildly useful trick
for ruthlessly detecting subtle operating system mistakes.  It will
prove invaluable next week when we do virtual memory --- a topic that
has extremely hard-to-track-down bugs if you just use "normal" testing
of running programs and checking print statements and exit codes.


Today you'll build the pieces for the final step on thursday.  There's not
much code, we build it up in small pieces, and it tests itself, so that's
a nice improvement over some labs.

Goals:
  1. Be able to pre-emptively switch between user processes.
  2. Hash every single register on every single instruction 
     executed so that no difference can ever be missed.

---------------------------------------------------------------
## Part 0: reading and writing registers at different modes.

Here you'll learn how to read and write the user-level registers
from kernel mode using the `ldm` and `stm` idiom in an easy-to-test
and examine way.  You'll also write the code to read and write other
mode registers.  You'll use these as a basis for writing process and
exception switching code.

Two files:
  - Test harness: `0-user-sp-lr.c`.  You don't have to implement anything,
    though you should definitely insert assertions or print statements
    if things act weirdly!

  - All code to write: `0-user-sp-lr-asm.S`. 

Look at the comments in both files and implement each part at a time
(four parts in total).  This is a pretty mechanical fetch quest.

---------------------------------------------------------------
## Part 1: using `rfe` to switch to user level.

Here you'll implement different examples that use the `rfe` instruction.
The `rfe` instruction lets you simultaneously set both the `pc` and `cpsr`
at once and is a key part of how you context switch into a user process.

Unlike your thread context-switch code you can't switch into a user-level
process by simply setting the `pc` register because you *also* have to
change the `cpsr` to switch from privileged kernel mode to unprivileged
`USER` mode.  If you did this mode switch before jumping to user code
you'd get a privileged fault (since the `pc` would still be in kernel
memory).  And doing it after has similar issues. The `rfe` instruction
lets you set both at once.

To see how `rfe` works:
  1. RTFM the manual.
  2. Then look at the complete example `1-rfe-example.c`, with associated
     assembly code in: `1-rfe-asm.S:rfe_asm` and
     `1-rfe-asm.S:rfe_trampoline_asm`.  Make sure the comments and output
     make sense or the next 6 lines of code could easily take a few hours.

There are two tests.
Since messing up assembly can be hard to debug, we split restoring
registers into
one small change (test 1), and then a larger one (test 2):

 1. `1-rfe-blk.c`.  This changes `1-rfe-example.c` in a minor way
    to take a 17-entry array (as you would use with process switching)
    instead of a 2-entry one.

    What to do: You should write the code `1-rfe-asm.S:blk_rfe_asm`
    to handle a 17-entry array with the `pc` at offset 15, and the
    `cpsr` you want to restore at offset 16.  It's a trivial change,
    but you want this correct before doing the next step.

 2. `1-rfe-switchto-user.c`.  This sets all the registers in the
    17-entry block and eliminates the need for a trampoline used in
    `1-rfe-asm.S` to setup the stack pointer.

    What to do: You'll write `1-rfe-asm.S:switch_to_user_asm` to load
    user mode registers `r0-r14` and then do an `rfe`.  This is just
    a matter of copying and modifying some of your part 1 assembly.

    ***Note, the test in its current form only validates `r0-r3`, `sp`,
    `pc` and mode (we will do the others below).***

At this point you have a partially validated register switch: we'll
do the rest of it in the next few stages.

---------------------------------------------------------------
## Part 2: saving all user registers.

Here you'll implement the assembly to save all registers in ascending
order into a single 17-entry block of memory on the exception stack and
pass that to the system call exception handler.  This is the other half
of getting to full processes.

***Note: if you get a compiler error for `vector-base.h`  you need to put
your `vector-base.h` in `libpi/src` and it should take care of it.***

There are two different tests:
  1. The simplest test harness: `2-reg-save.c` that uses the `rfe_asm`
     above to run user code that calls a system call handler.

     The code to implement: `2-reg-save-asm.S:`swi_trampoline`.  This
     should save all registers into a 17 entry block on the interrupt
     stack and pass the base of this array into the system call routine
     which will simply print them and reboot.  Make sure you increment
     and compute offsets correctly.

     The idea here is that that we run `staff-start.S:mov_ident` which
     loads each register with its number (r1 has 1, r2 has 2, etc)
     and then calls our system call.  This makes saving easy to debug:
     `r0-r14` in the 17-entry register block should have its offset
     as a value.  `r15` should be the pc of the swi instruction and
     r16 should have `USER` as a mode.

  2. The more fancy testing harness: this works similar to above,
     but it does not use `mov_ident` but instead initializes a 17-entry
     register block and then uses your `switchto_user_asm` (form part 1)
     to load the registers and jump to the swi instruction directly.
     Output should be the same as the first test.  If any register
     is off, you should be able to see.

At this point you have a partially tested set of user save and restore
routines.  We will now verify them using ministep without much work and
to a degree you'd be surprised if they are broken.  (I would be, but
I would not be without passing the next step.)

Great, now you have the main building blocks: we'll put all the pieces 
above together on Thusrday, hopefully in a way
that blows your mind.

---------------------------------------------------------------
## Part 3: saving and restoring privileged registers.

***Major confusion***:
  - If you use `msr` you'll notice the 8th bit in `cpsr` is set, but if
    you use `rfe` it is not.     This appears to be a quirk in the
    GNU assembler.  If you want to force `msr` to set all the bits it
    appears you need to have the following suffixes:

            msr   cpsr_cxsf, r1

    Or the GNU assembler will emit machine code that does not set it.
    For today we'll take the 8th bit set or not set.

For this final part, you'll write code to save and restore registers when
you're coming from and going to privileged (not user mode).  You should
make copies of the tests above (with appropriate renaming) and modify
them to go between some non-user mode (presumably not `SUPER` or `SYS`).
You already have the pieces for this so it's mainly a re-enforcement.


A couple of notes:
  1. The challenge here is that the caret operator `^` *only* loads
     or stores to user mode registers, not to any other (privileged)
     mode, so won't help us here.  So, since you don't have caret,
     what do you do?   The good thing is that `0-user-sp-lr-asm.S`
     has most of the pieces already so you can just use those.

  2. In some sense it is easier to save and restore state between
     privileged modes versus user mode since we can switch back and
     forth however we want, whereas once we go to user mode we need a
     system call to get back.  

  3. Probably the easiest approach to restore the privileged registers
     is to (1) do a full `msr` switch to the right mode (as in
     `libpi/staff-start.S`) and then (2) do a `ldm` of all the registers.

  4. Also, and this is my fault: your privileged mode save and restore
     should work for any privileged mode, not just a specific hardcoded
     one.  So you should use `msr` not `cps`.



***NOTE: you are allowed to do the following easier, equivalant way 
that requires less thinking***

  1. Keep the `swi_trampoline` as-is from part 2.  It assumes
     you assumes you came from user mode.  Thus, for the part 3 test
     it will save everything correctly *except* `sp` and `lr` (since it
     assumed we came from the wrong place).  So we will just fix these
     up in C code (next step).

  2. In the C code for `do_syscall`: check which mode you came from using
     the passed in register array `regs` (`cpsr` should be stored
     at `regs[16]`).

     If you came from USER keep running as before since the registers 
     are saved correctly. 

     If you didn't come from USER, then you want to patch the `sp` and
     `lr` locations in the reg array (`&regs[13]` and `&regs[14]`).

     This is easy because you already wrote a routine that will
     get these two registers from an arbitrary privileged mode:

            // 0-user-sp-lr-asm.S
            void mode_get_lr_sp_asm(uint32_t mode, uint32_t *sp, uint32_t *lr)

     Just call it to fix things up using the `cpsr` value saved in
     the `reg` array (`reg[16]`).

-----------------------------------------------------------------------
### Check off your final project and spec out parts, todo.

You should:
  1. Check off your project description;
  2. Spec out parts;
  3. Get the datasheets and see that they are reasonable;
  4. Have a rough todo.

A couple hours now will save you later.

<p align="center">
  <img src="images/pi-goofy.png" width="700" />
</p>
