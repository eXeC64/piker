#include "kernel.h"

#include "uart.h"
#include "interrupts.h"
#include "timer.h"
#include "frame.h"
#include "page.h"
#include "tasks.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void)r0;
    (void)r1;
    (void)atags;
    timer_init();
    uart_init();
    frame_init();
    interrupts_init();
    tasks_init();

    uint32_t r;
    __asm volatile("mrc p15, 0, %0, c1, c1, 0" : "=g" (r));
    uart_printf("scr: 0x%x = 0b%b\n", r, r);

    uintptr_t pt = 0;
    pagetable_init(&pt);

    uintptr_t frame;
    frame_alloc(&frame);

    pagetable_map_page(pt, 0x7000, frame);

    pagetable_activate(pt);

    __asm volatile(
            "ldr r0, =0xdead;"
            "ldr r1, =0xbeef;"
            "ldr r2, =0xabad1dea;"
            "ldr r3, =0x1234;"
            "svc %0;"
            : :
            "g" (0x80)
            : "r0", "r1", "r2", "r3"
            );

    size_t i = 0;
    while(TRUE) {
        uart_printf("time: %ims\n", (uint32_t)(timer_now()));
        uart_printf("i: %i\n", i++);

        mem_write(frame, 0xDEADBEEF);

        /*
        uart_printf("written\n");

        uart_printf("val: 0x%x\n", mem_read(0x7000));

        uart_printf("RAW: 0x%x\n", mem_read(ARM_TIMER_IRQ_RAW));
        uart_printf("MASK: 0x%x\n", mem_read(ARM_TIMER_IRQ_MASK));
        uart_printf("PEND: 0x%x\n", mem_read(ARM_IRQ_PENDING_BASIC));
        */

        timer_sleep(100);
    }

    uart_puts("\n*** HALTING ***\n");
}
