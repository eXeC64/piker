#include "kernel.h"

#include "tasks.h"

#define MAX_TASK 100

struct task tasks[MAX_TASK];

/* Prepare the system for multi-tasking */
void tasks_init(void)
{
    /* Initialise the task list */
    for(uint32_t i = 0; i < MAX_TASK; ++i)
      tasks[i].flags = 0;

    /* TODO: Create the main task */
}

/* This is the core of the scheduler. Here we update our books and pick which
 * task to run next.
 */
void tasks_tick(void)
{
  /* TODO */
  /* pick the next task to run */

  /* save the state of the current task */

  /* load the next task */
}
