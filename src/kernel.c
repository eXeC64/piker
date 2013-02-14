#include "kernel.h"

#include "uart.h"
#include "timer.h"
#include "mem/frame.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    uart_init();
    uart_puts("Piker 0.0.0\n");

    uart_puts("Setting up frames\n");

    frame_init();
    alloc_init();

    uart_printf("kernel_main address: 0x%x\n", (uint32_t)kernel_main);
    uart_printf("kernel_main val: 0x%x\n", mmio_read((uint32_t)kernel_main));

    uart_printf("Allocating 256 bytes of memory\n");
    uint32_t ptr = mem_alloc(256);
    uart_printf("mem: 0x%x\n", ptr);

    uart_printf("Allocating another 512 bytes of memory\n");
    uint32_t ptr2 = mem_alloc(512);
    uart_printf("mem: 0x%x\n", ptr2);

    uart_printf("Allocating another 256 bytes of memory\n");
    uint32_t ptr3 = mem_alloc(256);
    uart_printf("mem: 0x%x\n", ptr3);


    while(TRUE) {
        uart_printf("time: %ims\n", (uint32_t)(timer_now() / 1000));
        timer_sleep(1000);
    }

    uart_puts("\n*** HALTING ***\n");
}
