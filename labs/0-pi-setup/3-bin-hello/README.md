Trivial test to check that:
  1. Your tool chain will compile.
  2. The path to your bootloader <pi-install>  resolves correctly.
  3. The bootloader can find your tty-USB device.  If you have to manually
     manually specify your usb device to `pi-install`, set `TTYUSB`
     to the device path in `Makefile`.  E.g.,

            TTYUSB = /dev/ttyUSB0

  4. Your pi works: should print out "hello from pi" along with some
     debugging output.  For example:

            BOOT:simple_boot: sending 1404 bytes, crc32=a555db78
            BOOT:waiting for a start
            BOOT:bootloader: Done.
            hello world from the pi
            DONE!!!
