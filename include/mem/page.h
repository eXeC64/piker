#ifndef PAGE_H
#define PAGE_H

#include "types.h"

/* allocates and initialises a blank pagetable*/
int8_t pagetable_init(uintptr_t* pt);

/* frees the page table */
int8_t pagetable_free(uintptr_t pt);

/* maps addr to frame in pagetable page, null frame unmaps */
int8_t pagetable_map_page(uintptr_t pt, uintptr_t addr, uintptr_t frame);

/* activates a pagetable, mapping it into memory effective immediately */
int8_t pagetable_activate(uintptr_t pt);

#endif /*PAGE_H*/
