#include "kernel.h"

#include "mem/mmio.h"

void inline mem_flush_cache() {
    __asm__(
            "mov r3, #0;"
            "mcr p15, 0, r3, c7, c6, 0;"
            "mcr p15, 0, r3, c7, c10, 0;"
            "mcr p15, 0, r3, c7, c14, 0;"
            "mcr p15, 0, r3, c7, c10, 4;"
            "mcr p15, 0, r3, c7, c10, 5;"
            : /* no outputs */
            : /* no inputs */
            : "r3" /* we clobbered r3 */
           );
}

void inline mem_barrier() {
    __asm__(
            "mov r3, #0;"
            "mcr p15, 0, r3, c7, c5, 0;"
            "mcr p15, 0, r3, c7, c5, 6;"
            "mcr p15, 0, r3, c7, c10, 4;"
            "mcr p15, 0, r3, c7, c5, 4;"
            : /* no outputs */
            : /* no inputs */
            : "r3" /* we clobbered r3 */
           );
}

void inline mem_write(uint32_t address, uint32_t val) {
    *((uint32_t*)address) = val;
}

uint32_t inline mem_read(uint32_t address) {
    return *((uint32_t*)address);
}

void inline mem_set(uint32_t address, uint32_t len, uint32_t val) {
    for(uint32_t ptr = address; ptr < address + len; ptr++) { *((uint32_t*)ptr) = val; }
}

void inline mem_copy(uint32_t src, uint32_t dst, uint32_t len) {
    for(uint32_t offset = 0; offset < len; offset++) {
        *((uint32_t*)(dst + offset)) = *((uint32_t*)src + offset);
    }
}
