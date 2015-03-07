#include "kernel.h"

#include "mmio.h"

void inline mem_flush_cache()
{
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

void inline mem_barrier()
{
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

void inline mem_write(uintptr_t address, uint32_t val)
{
    *((uint32_t*)address) = val;
}

uint32_t inline mem_read(uintptr_t address)
{
    return *((uint32_t*)address);
}

void inline mem_set(uintptr_t address, size_t len, uint8_t val)
{
    for(uintptr_t ptr = address; ptr < address + len; ptr++) { *((uint8_t*)ptr) = val; }
}

void inline mem_copy(uintptr_t src, uintptr_t dst, size_t len)
{
    for(uintptr_t offset = 0; offset < len; offset++) {
        *((uint8_t*)(dst + offset)) = *((uint8_t*)src + offset);
    }
}
