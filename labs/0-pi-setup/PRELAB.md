# Prelab: Setup 

### Always obey the first rule of PI-CLUB

- **_IF YOUR PI GETS HOT TO THE TOUCH: UNPLUG IT_**
- **_IF YOUR PI GETS HOT TO THE TOUCH: UNPLUG IT_**
- **_IF YOUR PI GETS HOT TO THE TOUCH: UNPLUG IT_**
- **_IF YOUR PI GETS HOT TO THE TOUCH: UNPLUG IT_**
- **_IF YOUR PI GETS HOT TO THE TOUCH: UNPLUG IT_**

You likely have a short somewhere and in the worst-case can fry your laptop.

### tl;dr

For this lab, we'll use different variations of a blinky light to check
that parts of your toolchain / understanding are working.

You'll likely forget this link, but if at any point you get confused
about some basic concept, cs107e has a good set of guides on electricity,
shell commands, pi stuff in general:

- [CS107E repo] (http://cs107e.github.io/guides/)


Before lab, you should do the following (described in the rest of 
the PRELAB):

0.  Make sure you know how to use common unix commands such as: `ls`,
    `mkdir`, `cd`, `pwd`.
1.  Figure out how to add a local `~/bin` directory in your home directory
    to your shell's `PATH` variable.
2.  Have a way to mount a micro-SD card on your laptop.
3.  Install software as necessary based on 
    the [software guide](setup/SOFTWARE.md).

------------------------------------------------------------------------
## 1. change your `PATH` variable

To save you time, typing and mistakes: tell your shell to look for
executable programs in a `bin` directory located in your home directory
(i.e., `~/bin`).

We suggest you figure out how to do this on your own, but if you 
get stuck, here's a bit more information:


  1. For whatever shell (`tcsh`, `bash`, etc) you are using, figure
     out how to edit your `PATH` variable so that you can install binary
     programs in a local `~/bin` directory and not have to constantly
     type out the path to them.  E.g.,

           % cd ~          # change to your home dir
           % mkdir bin     # make a bin directory
           % cd bin        # cd into it
           % pwd           # get the absolute path
           /home/engler/bin

     To figure out your shell, you can typically check in `/etc/password`:

           % grep engler /etc/passwd
           engler:x:1000:1000:engler,,,:/home/engler:/usr/bin/tcsh

     Since I use `tcsh`, to add `/home/engler/bin` to my `path` I would
     edit my `~/.tcshrc` file and change:

          set path = ( $path  )

     To:

          set path = ( $path /home/engler/usr/bin )

     and then tell the shell to re-scan the paths as follows:

           % source ~/.tcshrc

     Note, you have to run `source` in every open window:  it may be
     easier to just logout and log back in.

     For MacOS users using `bash`, put the path to your `~/bin` directory
     in a `.bashrc` file in your home directory. Just run:

           % vim ~/.bashrc

     When you add your path, make sure you include you default path or
     you may mess up your terminal.  This looks like:

           export PATH="$PATH:your/path/here:your/other/path/here"

      and source it the same way as the `.tcshrc`.  The instructions for `zsh`
      (the default on some newer Macs) are the same as for `bash`, but edit
      `~/.zshrc` instead.

------------------------------------------------------------------------
## 2. Micro-SD card stuff

In order to run code on the pi, you will need to be able to write to a
micro-SD card on your laptop:

1.  Get/bring a micro-SD card reader or adaptor if its not built-in
    to your laptop. The [CS107E guide on SD
    cards](http://cs107e.github.io/guides) is a great description
    (with pictures!).

2.  Figure out where your SD card is mounted (usually on MacOS it is in
    `/Volumes` and on linux in `/media/yourusername/`, some newer linuxes
    might put it in `/run/media/yourusername/`). Figure out
    how to copy files to the SD card from the command line using
    `cp`, which is much much faster and better than using a gui (e.g.,
    you can put it in a
    `Makefile`, or use your shell to redo a command). For me,

          % cp kernel.img /media/engler/0330-444/
          % sync

    will copy the file in `kernel.img` in the current directory to the
    top level directory of the mounted SD card, the `sync` command forces
    the OS to flush out all dirty blocks to all stable media (SD card,
    flash drive, etc). At this point you can pull the card out.

    Pro tip: **_do not omit either a `sync` or some equivalant file
    manager eject action_** if you do, parts of the copied file(s)
    may in fact not be on the SD card. (Why: writes to stable storage
    are slow, so the OS tends to defer them.)

------------------------------------------------------------------------
## 3.  Software Installation

Install the software in [software guide](setup/SOFTWARE.md).
