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

  pagetable_activate(pt);
  uart_printf("pagetable on\n");

  uart_printf("main loop on\n");
  uart_printf("char=%i\nshort=%i\nint=%i\nlong=%i\nptr=%i\n",
      sizeof(char), sizeof(short), sizeof(int), sizeof(long), sizeof(void*));
  while(TRUE) {

    uart_printf("time: %ims\n", (uint32_t)timer_now());

    timer_sleep(1000);
  }

  uart_puts("\n*** HALTING ***\n");
}
