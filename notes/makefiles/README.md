### Intro

As projects get large, manually compiling code is a pain.   The Unix
`make` program is one of the oldest ways to automate this process.
`make` has a deserved reputation as an unusually ugly and baroque
input language.  However, it exists everywhere, is used many places,
and has enough power embedded inside that you can often make it do what
you want --- compiling based on what changes with automatic depencies,
producing muiltiple programs or libraries and placing them in
different locations, automatically running regression tests, etc.

We use `make` alot.  Many places you might work or found will as well.
Despite its baroque weirdness, there is a narrow  slice that will do
most of what you need.  We cover much of that slice today by doing
increasingly fancy varions of a `Makefile` that does two things:

  1. Produces an executable file `main` by compiling the C source
     files `a.c`, `b.c`, `c.c`, `main.c`.

     Our makefile structure is driven by two correctness rules.

     Correctness rule 1: any source file must be recompiled if
     it changes.  The `main` must also be re-generated.
  
     Correctness rule 2: All of the example files `#include` a header file
     `header.h` and so must be recompiled if `header.h` changes.

     In general, a source file must be recompiled if anything it depends
     on changes.  For today we only worry about the header `header.h`.
     Of course, the makefile itself is a dependency.  

     In the limit, the operating system installation, compiler
     (assembler, linker, preprocessor) or libc implementations can be
     seen as dependencies as well ---- in some cases you may want to
     recompile if any of these changes.  We do not do this today.

  2. After producing the `main` executable, check that running `main`
     gives the same result as a reference output `main.ref`.

Required reading:
  - [make wikipedia page](https://en.wikipedia.org/wiki/Make_(software)).
  - [simple concrete makefiles](http://nuclear.mutantstargoat.com/articles/make/).
    While the domain name is weird, the `make` examples are concrete,
    simple, useful.

### Examples:

We give two main examples of `make`:
  1. `0-basic-unix-makefile` which works through everything
     in a simple way.  The resultant makefile should work
     for most of your projects.
  2. `make-using-eval`: which is a fancier version that is
      likely the best robust approach for large projects through
      its use of the `eval` function in `GNUmake` to generate
      explicit compilation rules.  This makefile should work
      for any project you do, afaik. 

### tl;dr of common mistakes

Common make mistakes that have burned us that the example
makefiles address:

  - Mispell `make` variables.  this is especially a problem when the
    variable is used as a compilation rule's dependency since it will
    be empty, and thus doesn't give a dependency.  The first `make` will
    work correctly (since it builds from scratch) as will a `make` after
    `make clean`, but later ones can use stale .o's after modifications.

    Solution: be careful.  Also when writing makefiles add the warning
    for unused variables to `make`'s flags:

        MAKEFLAGS += --warn-undefined-variables

  - Give the wrong path to where you have the automatically
    generated `.d` dependency files when you `include` them in
    the `Makefile`: `make` will silently proceed, and you have no
    dependencies.  Again: will work the first time or after make clean
    but will allow stale results later.

    Solution: add each `.d` as an explicit dependency for each `.o` rule.
    If the `.d` doesn't exist, will get an error.

  - Makes automatic rules do the wrong thing (e.g., and avoid the 
    `.d` safeguard above).

    Solution: disable built-in rules either individually or
    (preferred) en masse:
    
        # disable all built-in rules.
        .SUFFIXES:

  - Change the makefile flags but forget to `make clean; make`.
    Often fine, sometimes bad.  

    Solution: add a dependency on the `Makefile` itself.  Or do the hack
    of automatically running `make clean` when the makefile is newer
    than a timestamp file.  (We have examples of both.)

  - A file appears more than once in the directories `VPATH` contains.
    First resolution will win, no matter if you intended to grab a
    later one.

    Solution: don't use `VPATH`!  Explicitly generate the rules using
    `eval`, or have a build directory that mirrors the source directory,
    or generate the `.o` in the same directory as each source file (ugly,
    but simple).

  - `make` is deleting intermediate files (e.g., `.list` files)

    Wrong solution: add a .PRECIOUS directive:

        .PRECIOUS: %.list %.elf 

    This will leave garbage if you control-c during `make`.

    Correct solution: explicitly list out the files that are produced
    by rules.  This also partially checks that your internal rules
    work correctly.
