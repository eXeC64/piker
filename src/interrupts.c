#include "kernel.h"

#include "interrupts.h"
#include "uart.h"


__attribute__ ((naked)) void int_halt() { while(1); }

__attribute__ ((interrupt ("SWI"))) void int_swi() {
    uint32_t next_ins, swi;
    __asm volatile("mov %0, lr" : "=r" (next_ins));
    swi = *((uint32_t*)next_ins - 1) & 0x00FFFFFF;

    /* Handle SWI */
    uart_printf("Received SWI %i returns to 0x%x\n", swi, next_ins);
}

__attribute__ ((naked, aligned(32))) static void vectors(void) {
    __asm volatile("b int_halt;"
                   "b int_halt;"
                   "b int_swi;"
                   "b int_halt;"
                   "b int_halt;"
                   "b int_halt;"
                   "b int_halt;"
                   "b int_halt;"
                   );
}

int8_t interrupts_init() {
    /* Install vectors and enable interrupts */
    __asm volatile("mcr p15, 0, %0, c12, c0, 0" : : "r" (&vectors));
    return 0;
}
