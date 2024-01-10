// common patten: 
//  1. Before doing an operation, spin on status flag
//     in device memory (magic address) until it is "ready".
//  2. Once the device is ready, do the operation.
//
//  problem: compiler does not know the device can write memory.
typedef struct {
  unsigned read;
  unsigned padding[3];
  unsigned peek;
  unsigned sender;
  unsigned status;
  unsigned configuration;
  unsigned write;
} mailbox_t;

#define MAILBOX_EMPTY  (1<<30)
#define MAILBOX_FULL   (1<<31)

static mailbox_t *mbox =  (void*)0x2000B880;

void write_mailbox(volatile void *data, unsigned channel) {
    // what happens here?
    while(mbox->status & MAILBOX_FULL)
        ;
    mbox->write = ((unsigned)(data) | channel | 0x40000000);
}
void inf(void) { 
    while(1); 
}
