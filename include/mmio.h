#ifndef MMIO_H
#define MMIO_H

#include <stdint.h>

void mmio_write(uint32_t address, uint32_t val);
uint32_t mmio_read(uint32_t address);

void mmio_memset(int32_t* address, uint32_t len, int32_t val);
void mmio_memcpy(int32_t* src, int32_t* dst, uint32_t len);

#endif
