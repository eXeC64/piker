#include "kernel.h"

#include "tasks.h"

/* Prepare the system for multi-tasking */
int8_t tasks_init() {

    mem_barrier();

    /* Enable arm timer irq */
    mem_write(ARM_IRQ_ENABLE_BASIC, 0x1);

    /* Set countdown to 0x400 ticks */
    mem_write(ARM_TIMER_LOAD, 0x4000);

    /* Enable timer, timer interrupt, scale clock by / 0x100, 32 bit counter */
    mem_write(ARM_TIMER_CONTROL, 0xAA);

    return 0;
}
