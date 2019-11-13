#include "kernel.h"

#include "page.h"
#include "frame.h"
#include "uart.h"


int pagetable_init(struct pagetable **pt)
{
    if (frame_alloc((void**)pt) != 0) {
        return -ENOMEM;
    }

    mem_set((uintptr_t)*pt, 4096, 0);

    return 0;
}

void pagetable_free(struct pagetable *pt)
{
    uintptr_t flda;
    for (flda = (uintptr_t)pt; flda < (uintptr_t)pt + 0x1000; flda += 4) {
        uint32_t fld = mem_read(flda);
        if((fld & 0x3) == 0x1) {
            uintptr_t slp = fld & 0xFFFFFC00;
            frame_free((void*)P2V(slp));
        }
    }

    frame_free(pt);
}

/* maps addr to frame in pagetable page, null frame unmaps */
int pagetable_map_page(struct pagetable *pt, void *virt, void *frame)
{
  return pagetable_map_page_phy(pt, virt, V2P(frame));
}

int pagetable_map_page_phy(struct pagetable *pt, void *virt, void *phy)
{
    uintptr_t frame = (uintptr_t)phy;

    uintptr_t flp = 0xFFFFF000 & V2P((uintptr_t)pt);

    uintptr_t flda = flp | (((uint32_t)virt >> 18) & 0xFFC);

    uint32_t fld = mem_read(P2V(flda));

    uintptr_t vslp;
    uintptr_t slp;

    if ((fld & 0x3) != 0x1) {
        if (frame_alloc((void**)&vslp) != 0) { return -ENOMEM; }
        slp = V2P(vslp);
        mem_set(vslp, 4096, 0);

        fld = (slp & 0xFFFFFC00) | 0x01;
        mem_write(P2V(flda), fld);
    } else {
        slp = 0xFFFFFC00 & fld;
    }

    uintptr_t slda = slp | (((uintptr_t)virt >> 10) & 0xFF);

    uint32_t sld;

    if (frame == 0) {
        sld = 0;
    } else {
        sld = frame | 0x72;
    }

    mem_write(P2V(slda), sld);

    /* Flush data cache and invalidate TLB entries */
    __asm volatile ("mcr p15, 0, %0, c7, c14, 0" : : "r" (0));
    __asm volatile ("mcr p15, 0, %0, c8,  c7, 0" : : "r" (0));

    return 0;
}

/* Defined in src/init.s */
extern struct pagetable *blank_pagetable;

void pagetable_activate(struct pagetable *pt)
{
    /* Null pagetable means unmap it all */
    if (pt == 0) {
        pt = blank_pagetable;
    }
    /* Load the page table into TTBR0 */
    __asm volatile ("mcr p15, 0, %0, c2, c0, 0" : : "r" (V2P((uint32_t)pt)));

    /* Invalidate the TLB entries */
    __asm volatile ("mcr p15, 0, %0, c8, c7, 0" : : "r" (0));
}

