#include "kernel.h"

#include "uart.h"
#include "timer.h"
#include "mem/frame.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    uart_init();
    uart_puts("Piker 0.0.0\n");

    uart_puts("Setting up frames\n");

    frame_init();

    uart_printf("kernel_main address: 0x%x\n", (uint32_t)kernel_main);
    uart_printf("kernel_main val: 0x%x\n", mmio_read((uint32_t)kernel_main));

    uint32_t my_frames[16];

    if(frame_alloc_mult(my_frames, 16)) {
        uart_printf("allocated frames:\n", my_frames);

        for(uint8_t i = 0; i < 16; ++i) {
            uart_printf("%i: 0x%x\n", i, my_frames[i]);
        }
    }

    while(TRUE) {
        uart_printf("time: %ims\n", (uint32_t)(timer_now() / 1000));
        timer_sleep(1000);
    }

    uart_puts("\n*** HALTING ***\n");
}
