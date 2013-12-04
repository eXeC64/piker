#ifndef TIMER_H
#define TIMER_H

#include "types.h"

uint64_t timer_now();

void timer_sleep(uint64_t duration_ms);

#endif /* TIMER_H */
