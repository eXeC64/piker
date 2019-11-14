#include "kernel.h"

#include "uart.h"
#include "interrupts.h"
#include "timer.h"
#include "frame.h"
#include "page.h"
#include "tasks.h"
#include "qa7_control.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
  (void)r0;
  (void)r1;
  (void)atags;
  timer_init();
  uart_init();
  uart_printf("uart on\n");
  frame_init();
  interrupts_init();
  uart_printf("interrupts on\n");
  tasks_init();
  uart_printf("tasks on\n");


  struct pagetable *pt = 0;
  pagetable_init(&pt);

  struct qa7_control *cnt = (struct qa7_control*)(QA7_BASE_ADDR);

  pagetable_activate(pt);
  uart_printf("pagetable on\n");


  cnt->local_intr_routing = 0;
  cnt->local_timer_flags = (1<<31) | (1<<30);
  const uint32_t INT_ENABLE = 1 << 29;
  const uint32_t TIMER_ENABLE = 1 << 28;
  cnt->local_timer_control = INT_ENABLE | TIMER_ENABLE | (10 * 1000 * 1000);

  uart_printf("main loop on\n");
  uart_printf("char=%i\nshort=%i\nint=%i\nlong=%i\nptr=%i\n",
      sizeof(char), sizeof(short), sizeof(int), sizeof(long), sizeof(void*));
  while(TRUE) {

    uart_printf("time: %ims\n", (uint32_t)timer_now());

    timer_sleep(1000);
  }

  uart_puts("\n*** HALTING ***\n");
}
