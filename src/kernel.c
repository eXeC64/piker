#include "kernel.h"

#include "uart.h"
#include "interrupts.h"
#include "timer.h"
#include "mem/frame.h"
#include "mem/page.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    uart_init();

    interrupts_init();

    uart_puts("Piker 0.0.0\n");

    frame_init();

    uintptr_t pt = 0;
    pagetable_init(&pt);

    uintptr_t frame;
    frame_alloc(&frame);

    pagetable_map_page(pt, 0x7000, frame);

    pagetable_activate(pt);

    while(TRUE) {
        uart_printf("time: %ims\n", (uint32_t)(timer_now() / 1000));

        mem_write(frame, 0xDEADBEEF);

        uart_printf("written\n");

        uart_printf("val: 0x%x\n", mem_read(0x7000));

        timer_sleep(1000);
    }

    uart_puts("\n*** HALTING ***\n");
}
