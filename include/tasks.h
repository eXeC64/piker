#ifndef TASKS_H
#define TASKS_H

#include "types.h"

void tasks_init();
void tasks_tick();

#define TASK_VALID    0x01 /* Set if the task is valid */
#define TASK_RUNNING  0x02 /* Set if the task is running */
#define TASK_PAUSED   0x04 /* Set if the task is paused */
#define TASK_NOPREMPT 0x08 /* Set if the task is immune to prempting */
#define TASK_SUPER    0x10 /* Set if the task should run in supervisor mode */

typedef struct {
  uint32_t flags;
  uintptr_t pagetable;
  uint32_t r[13];
  uint32_t sp;
  uint32_t lr;
  uint32_t pc;
} task_t;

#endif /* TASKS_H */
