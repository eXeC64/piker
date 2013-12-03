#ifndef MMIO_H
#define MMIO_H

#include <stdint.h>

void mem_flush_cache();
void mem_barrier();

void mem_write(uintptr_t address, uint32_t val);
uint32_t mem_read(uintptr_t address);

void mem_set(uintptr_t address, size_t len, uint8_t val);
void mem_copy(uintptr_t src, uintptr_t dst, size_t len);

#endif
