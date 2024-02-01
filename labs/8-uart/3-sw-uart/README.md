Write a sw-uart implementation.

Make sure everything works by:
  1. add `STAFF_OBJS += ./staff-objs/sw-uart.o` to your `Makefile`
  2. `make`
  3. `pi-install hello.bin` should emit:

            about to use the sw-uart
            sw_uart: hello world
            sw_uart: hello world
            sw_uart: hello world
            sw_uart: hello world
            sw_uart: hello world
            sw_uart: hello world
            sw_uart: hello world
            sw_uart: hello world
            sw_uart: hello world
            sw_uart: hello world
            done!

Then start writing `sw_uart_putc` in `sw-uart.c`:
  1. Change the Makefile to use the local one by uncommenting it.

have them write a sw_uart_get8 and a sw_uart_put8 that loops back with
a fairly low error rate.

have them write a timed write N cycles.
