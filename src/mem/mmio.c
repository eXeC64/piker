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
           );
}

void inline mem_barrier() {
    __asm__(
            "mov r3, #0;"
            "mcr p15, 0, r3, c7, c5, 0;"
            "mcr p15, 0, r3, c7, c5, 6;"
            "mcr p15, 0, r3, c7, c10, 4;"
            "mcr p15, 0, r3, c7, c5, 4;"
           );
}

void inline mmio_write(uint32_t address, uint32_t val) {
    *((uint32_t*)address) = val;
}

uint32_t inline mmio_read(uint32_t address) {
    return *((uint32_t*)address);
}

void inline mmio_memset(int32_t* address, uint32_t len, int32_t val) {
    for(int32_t* ptr = address; ptr < address + len; ptr++) { *ptr = val; }
}

void inline mmio_memcpy(int32_t* src, int32_t* dst, uint32_t len) {
    for(int32_t offset = 0; offset < len; offset++) {
        *(dst + offset) = *(src + offset);
    }
}
