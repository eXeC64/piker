#include "kernel.h"

#include "timer.h"

uint64_t timer_now() {
    mem_barrier();

    uint64_t low = mem_read(SYS_TIMER_CLO);
    uint64_t high = mem_read(SYS_TIMER_CHI);

    return (high << 32) | low;
}

void timer_sleep(uint64_t duration_ms) {
    uint64_t end = timer_now(NULL) + duration_ms * 1000;
    while(timer_now() < end) { ; }
}

