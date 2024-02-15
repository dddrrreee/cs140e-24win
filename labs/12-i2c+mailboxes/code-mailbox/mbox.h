#ifndef __MBOX_H__
#define __MBOX_H__

//
// rpi mailbox interface.
//  https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
//
//  a more reasonable but unofficial writeup:
//  http://magicsmoke.co.za/?p=284
//
// also good old valvers:
//      https://www.valvers.com/open-software/raspberry-pi/bare-metal-programming-in-c-part-5/#mailboxes
#include "rpi.h"

/***********************************************************************
 * mailbox interface
 */

// https://www.valvers.com/open-software/raspberry-pi/bare-metal-programming-in-c-part-5/#mailboxes
// https://elinux.org/RPi_Framebuffer
#define MAILBOX_FULL   (1<<31)
#define MAILBOX_EMPTY  (1<<30)
#define MAILBOX_START  0x2000B880
#define GPU_MEM_OFFSET    0x40000000

// document states: only using 8 right now.
#define MBOX_CH  8

/*
    REGISTER	ADDRESS
    Read	        0x2000B880
    Poll	        0x2000B890
    Sender	        0x2000B894
    Status	        0x2000B898
    Configuration	0x2000B89C
    Write	        0x2000B8A0
 */
#define MBOX_READ   0x2000B880
#define MBOX_STATUS 0x2000B898
#define MBOX_WRITE  0x2000B8A0

// need to pass in the pointer as a GPU address?
static inline uint32_t uncached(volatile void *cp) { 
    // not sure this is needed: we have no data caching
    // since no VM
	return (unsigned)cp | GPU_MEM_OFFSET; 	
}

static inline void 
mbox_write(unsigned channel, volatile void *data) {
    assert(channel == MBOX_CH);
    // check that is 16-byte aligned
	assert((unsigned)data%16 == 0);

    // 1. we don't know what else we were doing before: sync up 
    //    memory.
    dev_barrier();

    // 2. if mbox status is full, wait.
	while(GET32(MBOX_STATUS) & MAILBOX_FULL)
        ;

    // 3. write out the data along with the channel
	PUT32(MBOX_WRITE, uncached(data) | channel);

    // 4. make sure everything is flushed.
    dev_barrier();
}

static inline unsigned 
mbox_read(unsigned channel) {
    assert(channel == MBOX_CH);

    // 1. probably don't need this since we call after mbox_write.
    dev_barrier();

    // 2. while mailbox is empty, wait.
	while(GET32(MBOX_STATUS) & MAILBOX_EMPTY)
            ;

    // 3. read from mailbox and check that the channel is set.
	unsigned v = GET32(MBOX_READ);

    // 4. verify that the reply is for <channel>
    if((v & 0xf) != channel)
        panic("impossible(?): mailbox read for a different channel\n");

    // return it.
    return v;
}

static inline uint32_t 
mbox_send(unsigned channel, volatile void *data) {
    mbox_write(MBOX_CH, data);
    mbox_read(MBOX_CH);

    volatile uint32_t *u = data;
    if(u[1] != 0x80000000)
		panic("invalid response: got %x\n", u[1]);
    return 0;
}

// implement these!
uint32_t rpi_get_revision(void);
uint32_t rpi_get_model(void);
uint32_t rpi_clock_curhz_get(uint32_t clock);
uint32_t rpi_clock_realhz_get(uint32_t clock);
uint32_t rpi_clock_hz_set(uint32_t clock, uint32_t hz);
uint32_t rpi_clock_maxhz_get(uint32_t clock);
uint32_t rpi_clock_minhz_get(uint32_t clock);
uint32_t rpi_get_memsize(void);
uint64_t rpi_get_serialnum(void);

// get the temperature.
uint32_t rpi_temp_get(void) ;

#endif
