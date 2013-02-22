#include "kernel.h"

#include "mailbox.h"

#define MAILBOX_DATA_MASK 0xFFFFFFF0
#define MAILBOX_CHAN_MASK 0x0000000F

void mailbox_send(uint32_t data, uint32_t channel) {
    mem_barrier();

    while(0 != (mem_read(MAILBOX_0_STATUS) & (1 << 31))) { ; }

    uint32_t message = 0;
    message |= data & MAILBOX_DATA_MASK;
    message |= channel & MAILBOX_CHAN_MASK;

    mem_write(MAILBOX_0_WRITE, message);
}

uint32_t mailbox_read(uint32_t channel) {
    mem_barrier();
    channel &= MAILBOX_CHAN_MASK;

    while(TRUE) {
        while(0 != (mem_read(MAILBOX_0_STATUS) & (1 << 30))) { ; }

        uint32_t message = mem_read(MAILBOX_0_READ);
        uint32_t data = message & MAILBOX_DATA_MASK;
        uint32_t chan = message & MAILBOX_CHAN_MASK;

        if(chan == channel) {
            return data;
        }
    }
}


