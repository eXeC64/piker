#include "kernel.h"

#include "uart.h"
#include "timer.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    uart_init();
    uart_puts(STR("Piker 0.0.0\n"));

    while(TRUE) {
        uart_printf(STR("time: %ims\n"), timer_now() / 1000);
        timer_sleep(1000);
    }

    uart_puts(STR("\n*** HALTING ***\n"));
}
