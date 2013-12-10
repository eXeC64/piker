#include "kernel.h"

#include "syscalls.h"
#include "uart.h"

void handle_syscall(uint32_t call, int32_t r0, int32_t r1, int32_t r2, int32_t r3)
{
    uart_printf("SYSCALL %i\n", call);
    uart_printf("r0: 0x%x\nr1: 0x%x\nr2: 0x%x\nr3: 0x%x\n", r0, r1, r2, r3);
}
