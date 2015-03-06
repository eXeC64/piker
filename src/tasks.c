#include "kernel.h"

#include "tasks.h"

#define MAX_TASK 100

task_t tasks[MAX_TASK];

/* Prepare the system for multi-tasking */
void tasks_init()
{
    /* Initialise the task list */
    for(uint32_t i = 0; i < MAX_TASK; ++i)
      tasks[i].flags = 0;

    /* TODO: Create the main task */

    mem_barrier();

    /* Enable arm timer irq */
    mem_write(ARM_IRQ_ENABLE_BASIC, 0x1);

    /* Set countdown to 0x400 ticks */
    mem_write(ARM_TIMER_LOAD, 0x4000);

    /* Enable timer, timer interrupt, scale clock by / 0x100, 32 bit counter */
    mem_write(ARM_TIMER_CONTROL, 0xAA);
}

/* This is the core of the scheduler. Here we update our books and pick which
 * task to run next.
 */
void tasks_tick()
{
  /* TODO */
  /* pick the next task to run */

  /* save the state of the current task */

  /* load the next task */
}
