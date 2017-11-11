.section ".text"

.global timer_get_freq
timer_get_freq:
  mrc p15, 0, r0, c14, c0, 0
  bx lr

.global timer_get_count
timer_get_count:
  mrrc p15, 0, r0, r1, c14
  bx lr
