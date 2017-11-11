#ifndef TIMER_H
#define TIMER_H

#include "types.h"

void timer_init(void);

uint32_t timer_get_freq(void);
uint64_t timer_get_count(void);

uint64_t timer_now(void);
void timer_sleep(uint64_t duration_ms);

#endif /* TIMER_H */
