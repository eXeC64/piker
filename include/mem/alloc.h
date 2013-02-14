#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

#include <stdint.h>

void alloc_init();

void* mem_alloc(uint32_t size);
void mem_free(void* address, uint32_t size);

#endif /*MEM_ALLOC_H*/
