#include "kernel.h"

#include "uart.h"
#include "timer.h"
#include "mem/frame.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    uart_init();
    uart_puts("Piker 0.0.0\n");

    frame_init();

    while(TRUE) {
        uart_printf("time: %ims\n", (uint32_t)(timer_now() / 1000));
        timer_sleep(1000);
    }

    uart_puts("\n*** HALTING ***\n");
}
