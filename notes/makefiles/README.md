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

Both of these are written to guard against or eliminate the
common mistakes below.

### tl;dr of common mistakes

Common `make` mistakes (all have burned us):

  - Mispell `make` variables.  this is especially a problem when the
    variable is used as a compilation rule's dependency since it will
    be empty, and thus doesn't give a dependency.  The first `make` will
    work correctly (since it builds from scratch) as will a `make` after
    `make clean`, but later ones can use stale .o's after modifications.

    Solution: be careful (sorry; `make` sucks for this).  You can
    somewhat guard against this with how you write rules (below).
    Also when writing makefiles add the warning for unused variables to
    `make`'s flags:

        MAKEFLAGS += --warn-undefined-variables

  - Give the wrong path when using `include` for automatically
    generated `.d` dependency files.  `make` will silently proceed, and
    you have no dependencies.  Again: will work the first time or after
    `make clean` but will allow stale results later.

    Solution: add each `.d` as an explicit dependency for each `.o` rule.
    If the `.d` doesn't exist, will get an error.

  - `make` automatic rules do the wrong thing (e.g., avoid the 
    `.d` safeguard above).

    Solution: disable built-in rules either individually or
    (preferred) en masse:
    
        # disable all built-in rules.
        .SUFFIXES:

    This gives you complete control over the build.

  - Change the `Makefile` but forget to `make clean; make`.
    Often fine, sometimes bad.  

    Solution: add a dependency on the `Makefile` itself.  Or do the hack
    of automatically running `make clean` when the `Makefile` is newer
    than a timestamp file.  (We have examples of both.)

  - A file appears more than once in the list of directories `VPATH`
    contains.  First resolution will win, no matter if you intended to
    grab a later one.

    Solution: don't use `VPATH`!  Several alternatives:
      1. Explicitly generate the rules using `eval`.
      2. Have a build directory that mirrors the source directory.
      3. Generate the `.o` in the same directory as each source file 
         (ugly, but simple).  

      Geneerally all three mean that duplicate files that contain the
      same routine will show up with multiple definitions.

  - `make` is deleting intermediate files (e.g., `.list` files)

    Wrong solution: add a .PRECIOUS directive:

        .PRECIOUS: %.list %.elf 

    This will leave garbage if you control-c during `make`.

    Correct solution: explicitly list out the files that are produced
    by rules.  This also partially checks that your internal rules
    work correctly.

  - The build directory you want to put intermediate results in doesn't
    get created robustly.

    I used to use a target for the build dir, but this is really finicky
    for reasons I actually don't understand.  

        # <FORCE> forces the rule to run always.
        $(BUILD_DIR): FORCE
			mkdir -p $(BUILD_DIR)
    
        FORCE:

    Solution: just put the `mkdir -p $(BUILD_DIR)` with each rule.
    This is kind of ugly, but the way we do things there aren't many
    locations and it is guaranteed to always work.

