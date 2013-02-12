#include "kernel.h"

#include "uart.h"
#include "timer.h"
#include "mem/frame.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    uart_init();
    uart_puts("Piker 0.0.0\n");

    uart_puts("Setting up frames\n");

    frame_init();

    for(uint32_t i = 0x00000; i < 0x32000; i += 0x1000) {
        uart_printf("frame: 0x%x = %i\n", i, frame_get(i));
    }

    uart_printf("kernel_main address: 0x%x\n", (uint32_t)kernel_main);
    uart_printf("kernel_main val: 0x%x\n", mmio_read((uint32_t)kernel_main));
    while(TRUE) {
        uart_printf("time: %ims\n", (uint32_t)(timer_now() / 1000));
        timer_sleep(1000);
    }

    uart_puts("\n*** HALTING ***\n");
}
