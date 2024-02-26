## Fancy `Makefile` for a large project.

Current best way I know to 
  1. take a bunch of source spread across multiple directories (including
     directories with relative paths)
  2. have complete control over the rules for generating .o and .d 
     (through the use of eval).
  3. put the .o's in a build dir.
  4. put the .d's in a build dir.
  5. do not use vpath.  

Why:
  1. vpath will eventually cause hard-to-debug problems if the same file
     name appears in different directories.  ***even if this name is
     not explicitly included in your source list!***

  2. doing the hack of making the build directory mirror the source
     directory is a pain with relative paths to source go above the
     `Makefile` directory (e.g., as they do if we include lab code
     into libpi).

     you can convert to relative paths to absolute, but then you get
     these crazy long directory trees in your build dir.  there does not
     seem to be a nice way prune off the largest common directory prefix.

  3. the foundational problem is that afaik `make` does not let you map
     from a source file to its .o.  `make` rules require that you can
     go from .o to .c.  if you put the .o in a build dir it's unclear
     mechanically how to derive the source directory from multiple
     options.

  4. we use explicit rules to eliminate all sorts of problems where built-in
     rules allow `make` to falsely think it knows what to do.

AFAIK the basic `Makefile` in this directory is correct and has a good
defensive approach to all sorts of mistakes.  let me know if you find
any issues!
