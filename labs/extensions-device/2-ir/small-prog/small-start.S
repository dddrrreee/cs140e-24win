// must use this start when loading code, otherwise we'll change the 
// stack.
.section ".text.boot"

// Q: this start looks weird compared to cs140e-start.S: why?
.globl _start
_start:

    mov sp, #   0x8000000
    // Q: why is this a <b> rather than a <bl>?
    b notmain

#if 0
@ keep these here so that they are in the same location in every binary.
@ if you want to override them easily, can re-define the name in rpi-asm.h
.globl put32
.globl PUT32
put32:
PUT32:
    str r1,[r0]
    bx lr

.globl get32
.globl GET32

get32:
GET32:
    ldr r0,[r0]
    bx lr

.globl dev_barrier
dev_barrier:
        mcr p15, 0, r0, c7, c10, 4
        bx lr

#endif
