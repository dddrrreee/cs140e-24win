#include <stdint.h>

#if 0
void PUT32(unsigned addr, unsigned v);
unsigned GET32(unsigned addr);
void put32(volatile void *addr, unsigned v);
unsigned GET32(unsigned addr);
unsigned get32(volatile void *addr);
void dev_barrier(void);

#else

static inline void 
put32(volatile void *addr, unsigned v) {
    *(volatile uint32_t *)addr = v;
}
static inline void 
PUT32(unsigned addr, unsigned v) {
    *(volatile uint32_t *)addr = v;
}

static inline unsigned GET32(unsigned addr) {
    return *(volatile uint32_t *)addr;
}
static inline 
unsigned get32(volatile void *addr) {
    return *(volatile uint32_t *)addr;
}

static inline void dev_barrier(void) {
  asm volatile("mcr p15, 0, r0, c7, c10, 4");
}

#endif

// XXX: go through and do the bitfields for these.
struct aux_periphs {
    volatile unsigned
        /* <aux_mu_> regs */
        io,     // p11
        ier,

#       define CLEAR_TX_FIFO    (1 << 1)
#       define CLEAR_RX_FIFO    (1 << 2)
#       define CLEAR_FIFOs  (CLEAR_TX_FIFO|CLEAR_RX_FIFO)
        // dwelch does not write the low bit?
#       define IIR_RESET        ((0b11 << 6) | 1)
        iir,

        lcr,
        mcr,
        lsr,
        msr,
        scratch,

#       define RX_ENABLE (1 << 0)
#       define TX_ENABLE (1 << 1)
        cntl,

        stat,
        baud;
};


static struct aux_periphs * const uart = (void*)0x20215040;

static inline int uart_can_putc(void) {
    return (get32(&uart->stat) & 0b10) != 0;
}

static inline int uart_put8(uint8_t c) {
    dev_barrier();
    while(!uart_can_putc())
        ;
    put32(&uart->io, c & 0xff);
    dev_barrier();
    return 1;
}

static inline int uart_tx_is_empty(void) {
    return ((get32(&uart->stat) >> 9) & 0x1) == 1;
}

static inline void uart_flush_tx(void) {
    dev_barrier();
    while(!uart_tx_is_empty())
        ;
    dev_barrier();
}

static inline void rpi_reboot(void) {
    uart_flush_tx();

    // is there a way to speed this up?
    const int PM_RSTC = 0x2010001c;
    const int PM_WDOG = 0x20100024;
    const int PM_PASSWORD = 0x5a000000;
    const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;

    // timeout = 1/16th of a second? (whatever)
    PUT32(PM_WDOG, PM_PASSWORD | 1);
    PUT32(PM_RSTC, PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET);
    while(1);
}

static inline void delay(unsigned n) {
    while(n-- > 0)
        GET32(0);
}

static inline int putk(const char *p) {
    for(; *p; p++)
        uart_put8(*p);
    return 1;
}

// print out a special message so bootloader exits
static inline void clean_reboot(void) {
    putk("DONE!!!\n");
    rpi_reboot();
}

