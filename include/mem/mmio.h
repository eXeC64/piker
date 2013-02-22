#ifndef MMIO_H
#define MMIO_H

#include <stdint.h>

void mem_flush_cache();
void mem_barrier();

void mem_write(uint32_t address, uint32_t val);
uint32_t mem_read(uint32_t address);

void mem_set(int32_t* address, uint32_t len, int32_t val);
void mem_copy(int32_t* src, int32_t* dst, uint32_t len);

#endif
