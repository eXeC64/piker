#include "kernel.h"

#include "uart.h"
#include "timer.h"
#include "mem/frame.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    uart_init();
    uart_puts("Piker 0.0.0\n");

    frame_init();
    mem_alloc_init();

    uint32_t alloc_1 = mem_alloc(256);
    uart_printf("alloc_1 : 0x%x\n", alloc_1);

    uint32_t alloc_2 = mem_alloc(256);
    uart_printf("alloc_2 : 0x%x\n", alloc_2);

    uint32_t alloc_3 = mem_alloc(512);
    uart_printf("alloc_3 : 0x%x\n", alloc_3);

    mem_free(alloc_2);

    uint32_t alloc_4 = mem_alloc(256);
    uart_printf("alloc_4 : 0x%x\n", alloc_4);

    mem_free(alloc_1);

    uint32_t alloc_5 = mem_alloc(256);
    uart_printf("alloc_5 : 0x%x\n", alloc_5);

    mem_free(alloc_4);
    mem_free(alloc_5);

    uint32_t alloc_6 = mem_alloc(512);
    uart_printf("alloc_6 : 0x%x\n", alloc_6);

    while(TRUE) {
        uart_printf("time: %ims\n", (uint32_t)(timer_now() / 1000));
        timer_sleep(1000);
    }

    uart_puts("\n*** HALTING ***\n");
}
