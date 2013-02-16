#include "kernel.h"

#include "mailbox.h"

#define MAILBOX_DATA_MASK 0xFFFFFFF0
#define MAILBOX_CHAN_MASK 0x0000000F

uint32_t mailbox_buffer[256] __attribute__((aligned (16)));

void mailbox_send(uint32_t data, uint32_t channel) {
    mem_barrier();
    mem_flush_cache();

    while(0 != (mmio_read(MAILBOX_0_STATUS) & (1 << 31))) { mem_flush_cache(); }

    uint32_t message = 0;
    message |= data & MAILBOX_DATA_MASK;
    message |= channel & MAILBOX_CHAN_MASK;

    mmio_write(MAILBOX_0_WRITE, message);
}

uint32_t mailbox_read(uint32_t channel) {
    mem_barrier();
    channel &= MAILBOX_CHAN_MASK;

    while(TRUE) {
        mem_flush_cache();
        while(0 != (mmio_read(MAILBOX_0_STATUS) & (1 << 30))) { mem_flush_cache(); }

        uint32_t message = mmio_read(MAILBOX_0_READ);
        uint32_t data = message & MAILBOX_DATA_MASK;
        uint32_t chan = message & MAILBOX_CHAN_MASK;

        if(chan == channel) {
            return data;
        }
    }
}


