## Compilation lab

Unlike all subsequent labs, our first two don't use hardware.  They should
give a good feel for whether the class works for you without requiring
a hardware investment.

  - Today's short lab focuses on what happens when you compile code.
    How to automate compilation with `make`.  How to see how the compiler
    translated your C code by examining the machine code it produced.
    And some some of the subtle rules for how the compiler can do this
    translation.  What we cover will apply to every lab this quarter.

  - The next lab will dive into subtle inductive magic tricks that
    arise when you compile a compiler using itself.  FWIW: in the past,
    this lab was by far the favorite in the class post mortem.

What to do:
  0. Check out the class repository from github and setup the gcc
     cross-compiler.  See [how to setup the toolchain](./SOFTWARE.md)

  1. Read through the note on 
     [using gcc to figure out assembly code](../../notes/using-gcc-for-asm/README.md) and work through the examples.

     The ability to look at assembly to see how the compiler transformed
     your code (more on this in the next bullet) or to see how to do an
     act in assembly is a skill you'll use all quarter.  Not doing it
     was a pretty common initial mistake we saw last year.

     Many people get stuck really easily when they have to figure out
     how to write assembly code (reasonable since its new).  A simple
     hack to answering "how do I do X in ARM assembly?" is to do X
     in C code, use `gcc` to compile the C code to ARM machine code,
     and look at the result.  Using the compiler with active aggressive
     versus passively sitting and thinking will save you a bunch of time
     (not just in this class.)

  2. Read through the note on observability and compiler optimization:
     [observability](../../notes/observability/README.md) and work
     through the examples in `examples-pointer` and `examples-volatile`
     and then answer a few questions below.

     You should re-read this note carefully outside of class (several
     times!)  The compiler has very strong ideas about when and who
     can observe changes it makes to code.  Hardware devices typically
     wildly violate these assumptions.  Some of the hardest bugs that
     arise in the class (and IRL) come from ignorance of how and why
     these violations cause problems.

  3. Read through [makefile writeup](../../notes/makefiles/README.md) and go
     through the examples. 

The `docs` diretory has useful, further readings.
  
Checkoff:

  1. Write good makefiles for `examples-volatile` and `examples-pointer`.
     These should use wildcards and automatic dependencies.  After
     compiling a `.c` into a `.o` they should disassemble the result.

-------------------------------------------------------------------
### 1. using gcc to figure out assembly.

-------------------------------------------------------------------
### 2. write good makefiles for `notes/observability/examples-*`

If you look at the makefiles in these subdirectories in
`../../notes/observability' they are awful. You will fix them.

  1. Make copies of these directories in in the current lab.
  2.  Rewrite both `volatile/examples-pointer/Makefile` and `volatile/examples-volatile/Makefile` from scratch using the methods from:

      - [simple concrete makefiles](http://nuclear.mutantstargoat.com/articles/make/).

  3.  At the end of this lab, when you type `make` in your copies of
  `volatile/examples-pointer` or `volatile/examples-volatile`, all
  `.c`'s should be compiled into `.o`'s and corresponding disassembled
  `.dis`'s should be generated.

In particular:
  - Use wildcard patterns to get all the `.c` files from 
    the current directory and generate `.o` files from them.
  - Have a `CFLAGS`, `DIS` and `CC` variables so you can 
    switch between compiler, disassembly, and compiler options.
      - `CC` should be the name of your compiler (either `gcc`, `clang`, or `arm-none-eabi-gcc`)
      - `DIS` should be the name of your disassembler (either `objdump` or `arm-none-eabi-objdump`)
  - In the custom `%.o` rule you define, disassemble the resultant
    `.o`.
  - Use the built in make variables `$@` and and `$<`
  - Install the ARM compiler and make sure your makefile can correctly use it.

Note: `make` has some automatic rules to make `.o` files from `.c` files; make sure it's using your rules and not the built-in ones.

Additional reading:
  - [The wikipedia page gives a reasonable overview](https://en.wikipedia.org/wiki/Make_(software))
  - [cheatsheet](https://devhints.io/makefile)

Once you think you're done, please check in with one of the CA's (this is typical for labs--at the end of each lab you'll want to "check off" your work with a CA).
