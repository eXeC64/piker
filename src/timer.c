#include "kernel.h"

#include "timer.h"

uint32_t timer_freq = 0;

void timer_init()
{
  timer_freq = timer_get_freq();
}

uint64_t timer_now(void)
{
  return timer_get_count() / (timer_freq / 1000);
}

void timer_sleep(uint64_t duration_ms)
{
  uint64_t end = timer_now() + duration_ms;
  while(timer_now() < end) { ; }
}

