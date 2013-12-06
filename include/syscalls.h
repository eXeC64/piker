#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "types.h"

void handle_syscall(uint32_t call, int32_t r0, int32_t r1, int32_t r2, int32_t r3);

#endif /* SYSCALLS_H */
