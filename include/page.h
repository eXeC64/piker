#ifndef PAGE_H
#define PAGE_H

#include "types.h"

struct pagetable;

/* allocates and initialises a blank pagetable*/
int pagetable_init(struct pagetable **pt);

/* frees the page table */
void pagetable_free(struct pagetable *pt);

/* maps virt to frame in pagetable page, null frame unmaps */
int pagetable_map_page(struct pagetable *pt, void *virt, void *frame);
/* maps addr to physical address in pagetable */
int pagetable_map_page_phy(struct pagetable *pt, void *virt, void *phys);

/* activates a pagetable, mapping it into memory effective immediately */
void pagetable_activate(struct pagetable *pt);

#endif /*PAGE_H*/
