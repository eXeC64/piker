#include "kernel.h"

#include "interrupts.h"
#include "syscalls.h"
#include "uart.h"

__attribute__ ((naked)) void halt() { while(1); }

__attribute__ ((interrupt ("SWI"))) void int_swi(int32_t r0, int32_t r1, int32_t r2, int32_t r3) {
    uint32_t next_ins, swi;
    __asm volatile("mov %0, lr" : "=r" (next_ins));
    swi = *((uint32_t*)next_ins - 1) & 0x00FFFFFF;

    handle_syscall(swi, r0, r1, r2, r3);
}

__attribute__ ((interrupt ("ABORT"))) void int_prefetch_abort() {
    uart_printf("hit prefetch abort\n");
    halt();
}

__attribute__ ((interrupt ("ABORT"))) void int_data_abort() {
    uint32_t fault_addr, fault_status;
    __asm volatile("mrc p15, 0, %0, c6, c0, 0" : "=r" (fault_addr) );
    __asm volatile("mrc p15, 0, %0, c5, c0, 0" : "=r" (fault_status) );


    uart_puts("MMU DATA ABORT\n");
    uart_printf("fault address: 0x%x\n", fault_addr);
    uart_printf("fault status: 0b%b\n", fault_status);
    halt();
}

__attribute__ ((naked, aligned(32))) static void vectors(void) {
    __asm volatile("b halt;"
                   "b halt;"
                   "b int_swi;"
                   "b int_prefetch_abort;"
                   "b int_data_abort;"
                   "b halt;"
                   "b halt;"
                   "b halt;"
                   );
}

int8_t interrupts_init() {
    /* Install vectors and enable interrupts */
    __asm volatile("mcr p15, 0, %0, c12, c0, 0" : : "r" (&vectors));
    return 0;
}
