### Pitfalls in setting device state.

Device driver code often accesses device memory using 32-bit (or 64-bit)
`unsigned` pointers along with a set of `define`'s to shift and mask out
subfields in these 32-bit chunks.  This approach has the advantage that
code has a reasonable amount of control over what is happening. Further,
if you only need to modify a few bits, it can require few lines of
overall code.

However, as the number of fields adds up, so do the `define`s and so do
the mistakes.

A common alternative is to use C `struct` and bitfields to define the
layout of each device memory location.  While bitfields have tricky
portability issues, if you get the offsets correct (always do assertion
checks to verify!)  then this method has the nice feature that the
compiler will do the bit-operations for you.  

However, too many people seem completely unaware of how this approach
can cause pure evil errors. 

The core problem is that device datasheets implicitly make the
(reasonable) assumption that programs will modify fields grouped in the
same 32-bit memory block using a single atomic memory store.  They do not
contain guarentees that instead doing the modification using a 
sequence of memory operations gives a sensible result.  

This is easiest to see with an example.  Let's say we have a 32-bit
device register `r` that you encode using a C `struct` with bitfields:

    volatile struct r_send { 
        unsigned tx:1,   // offset=0: tx=1 --> transmit
                 :15,    // unused
                 len:16;    // offset=16: length of packet
    } *r = (void*)0x12345678;

Where to send a packet, the datasheet says to set the `tx` field to
1 to indicate the hardware should transmit and the `len` field to the
packet's length.

Let's assume you write the following natural code:

    r->len = 8;
    r->tx = 1;

Our ARM compiler generates the following:

    8010:   e3a02008    mov r2, #8
    8014:   e59f3010    ldr r3, [pc, #16]   ; 802c <notmain+0x1c>
    ; 1. store of r->len
    8018:   e583207c    str r2, [r3, #124]  ; 0x7c
    801c:   e1d327b8    ldrh    r2, [r3, #120]  ; 0x78
    8020:   e3822001    orr r2, r2, #1
    ; 2. store of r->tx
    8024:   e1c327b8    strh    r2, [r3, #120]  ; 0x78


Which, if you notice, does two different stores to set the location `r`
points to.

This is almost certainly undefined behavior as far as the datasheet's
device description is concerned: it almost certainly never states that
setting these two fields in sequence, as two distinct memory transactions,
gives guaranteed behavior of setting them "atomically" together with a
single store.  

This code *might* well work.  It also might not.  It might do both,
depending on various stuff (e.g., how quickly you emit these, how busy
the device is, etc.)

You might then decide to do the following:

    volatile struct r_send { 
        unsigned tx:1,   // offset=0: tx=1 --> transmit
                 :15,    // unused
                 len:16;    // offset=16: length of packet
    } r;

    r = *(volatile void*)0x12345678;
    r.tx = 1;
    r.len = 8;
    *(volatile void*)0x12345678 = r;
    
This has a much better shot of working as expected.  However, structure
assignment can be weird, and it can  be difficult tracking down the excact
guarantees that the compiler makes.  In particular, are we *absolutely
guaranteed* that the statement:

    *(volatile void*)0x12345678 = r;

Will generate a single store instruction?  It'd be surprised if not,
but I wouldn't bet my life on it.  For example, compilers often call
their internal `memcpy` to do struct assignment --- are you sure that
every single path through the compiler will respect that this statement
can be done with a 32-bit store?

Even if the standard suggests a `volatile` store should work this way,
we are not in the mainstream part of the language, so you can easily
run into disagreements in the compiler code or outright bugs.

   - NOTE: I am hopeful that we can do a lab pretty
     soon that implements memory tracing by protecting and then catching
     memory faults.  However, without this tool, we have no way to be
     sure other than manual inspection.

We have the same problem even when setting a single field and leaving 
the rest unaltered:

    void set_x(void) {
        volatile struct foo { 
            unsigned x:1,   // offset=0
                    :7,    // unused
                    y:24;   // offset=8
        } *f = (void*)0x12345678;

        f->x = 1;
        return;
    }


As above, the device datasheet may be written with the implicit assumption
of a 32-bit store of these values, but the compiler will almost certainly
implement this as a byte store of `x` rather than a a read-modify-write
of the entire 32-bit quantity (which can get hit with a cache miss).
This code may work, may only work for the current device version you have,
current optimization level you are doing, or any combination.

Last time I looked, cs107e code had a bunch of routines written in
this way.

Intead we will use the macros `get32_T` and `put32_T` and
the following idiom:

    // defined somewhere.
    struct r_send *addr = (void*)0x12345678;

    

    // read in the structure.
    struct r_send s = get32_T(addr);

    // modify privately.
    s.len = 8;
    s.tx = 1;

    // flush out.
    put32_T(r,s);

This guarantees we have a single load of the value, private modifications
and then a single store to write it out.  The code in `example/dev-struct.c`
has a bit more context.


