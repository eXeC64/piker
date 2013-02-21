#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

#include <stdint.h>

void mem_alloc_init();

uint32_t mem_alloc(uint32_t size);
void mem_free(uint32_t address);

#endif /*MEM_ALLOC_H*/
