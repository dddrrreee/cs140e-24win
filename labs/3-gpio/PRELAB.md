# Prelab: Setup and GPIO

### Always obey the first rule of PI-CLUB

- **_IF YOUR PI GETS HOT TO THE TOUCH: UNPLUG IT_**
- **_IF YOUR PI GETS HOT TO THE TOUCH: UNPLUG IT_**
- **_IF YOUR PI GETS HOT TO THE TOUCH: UNPLUG IT_**
- **_IF YOUR PI GETS HOT TO THE TOUCH: UNPLUG IT_**
- **_IF YOUR PI GETS HOT TO THE TOUCH: UNPLUG IT_**

You likely have a short somewhere and in the worst-case can fry your laptop.

---

## TL;DR

This is broken down some more below, but please make sure:

1. You've done the reading (see below), especially the Broadcom reading.
2. You've done the prelab questions on
   [Gradescope](https://www.gradescope.com/courses/488143/assignments/2568938).
3. You've installed the `arm-none-eabi-gcc` toolchain (see
   [software](setup/SOFTWARE.md)).
4. You have a way to read/write a microSD card and connect a USB-A device to
   your computer.
5. You've added `~/bin` to your `$PATH`.

There's more information below. Since this is the first lab and people have
varying levels of experience, we figured it's better to over-explain than
under-explain; future labs will be more succinct.

## Reading

1. Read through the [GPIO](./GPIO.md) and [device memory](./../../notes/devices/DEVICES.md) crash
   courses. You should have the [Broadcom
   document](../../docs/BCM2835-ARM-Peripherals.annot.PDF) open so you can go
   through the examples in the crash course
   (`../../docs/BCM2835-ARM-Peripherals.annot.PDF`).

2. After doing so, read through pages 4--7 and 91---96 of the broadcom
   document to see what memory addresses to read and write to get the GPIO pins
   to do stuff. The specific stuff we want to do: configure a pin as an output
   and turn it on and off, or configure it as an input and read its level.

   This is a low-level hardware document. It's okay if it's confusing! Just
   skim what you don't understand and try to pull out what you can. We will
   cover the necessary pieces in class.

3. Look through the `code` directory. You'll be implementing the routines in
   `gpio.c` which is used by three simple programs: `1-blink.c` `2-blink.c` and
   `3-input.c`. You only modify `gpio.c`. We provide prototypes (in `rpi.h`)
   and some trivial assembly routines in `start.S`.

4. Note: where the broadcom document uses addresses `0x7E20xxxx`, you'll use
   `0x2020xxxx`. The reason is complicated, but you can find a diagram
   explaining it on page 5 of the manual.

---

## Lab Info

1. You'll turn on an LED manually;
2. Then copy a pre-compiled program to an micro-SD and boot up your pi;
3. Then use a bootloader to ship the program to the pi directly;
4. Then install the r/pi tool chain, compile a given assembly
   version and use it;
5. Then write your own `blink` program, compile and run it;

Parts 1-4 are in `setup`, part 5 is in `code`.

You don't need to submit answers, but you should be able to answer the
following questions:

1. What is a bug that is easier to find b/c we broke up steps 1 and 2?
2. What is a bug that is easier to find b/c we broke up steps 2 and 3?
3. What is a bug that is easier to find b/c we broke up steps 3 and 4?
4. What is a bug that is easier to find b/c we broke up steps 4 and 5?

Differential debugging: Your pi setup in step 2 is not working.
Your partner's does. You plug your setup into their laptop.

1. Yours works in their laptop: what do you know?
2. Yours does not work in their laptop: what do you know?
