/*
 * "higher-level" neopixel stuff.
 * our model:
 *  1. you write to an array with as many entries as there are pixels as much as you 
 *     want
 *  2. you call <neopix_flush> when you want to externalize this.
 *
 * Note:
 *  - out of bound writes are ignored.
 *  - do not allow interrupts during the flush!
 */
#include "rpi.h"
#include "neopixel.h"
#include "WS2812B.h"

struct neo_pixel {
    uint8_t r,g,b;
};

// you can add other information to this if you want: this gives us a way to 
// control multiple light arrays concurrently, and to use different brands.
struct neo_handle {
    uint8_t pin;      // output pin
    uint32_t npixel;  // number of pixesl.

    // struct hack
    struct neo_pixel pixels[];
};

void neopix_flush(neo_t h) { 
    // needs the rest of the code.
    pix_flush(h->pin);
}

neo_t neopix_init(uint8_t pin, unsigned npixel) {
    neo_t h;
    unsigned nbytes = sizeof *h + sizeof h->pixels[0] * npixel;
    h = (void*)kmalloc(nbytes);
    memset(h, 0, nbytes);

    h->npixel = npixel;
    h->pin = pin;
    gpio_set_output(pin);
    return h;
}

// this just makes the definition visible in other modules.
void neopix_sendpixel(neo_t h, uint8_t r, uint8_t g, uint8_t b) {
    pix_sendpixel(h->pin, r,g,b);
}

// doesn't need to be here.
void neopix_fast_clear(neo_t h, unsigned n) {
    for(int i = 0; i < n; i++)
        pix_sendpixel(h->pin, 0,0,0);
    neopix_flush(h);
}

void neopix_clear(neo_t h) {
    neopix_fast_clear(h, h->npixel);
}

// set pixel <pos> in <h> to {r,g,b}
void neopix_write(neo_t h, uint32_t pos, uint8_t r, uint8_t g, uint8_t b) {
    unimplemented();
}
