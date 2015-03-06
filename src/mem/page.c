#include "kernel.h"

#include "mem/page.h"
#include "mem/frame.h"
#include "uart.h"


int8_t pagetable_init(uintptr_t* pt)
{
    if(frame_alloc(pt) != 0) {
        return -ENOMEM;
    }

    mem_set(*pt, 4096, 0);

    return 0;
}

void pagetable_free(uintptr_t pt)
{
    uintptr_t flda;
    for(flda = pt; flda < pt + 0x1000; flda += 4) {
        uint32_t fld = mem_read(flda);
        if((fld & 0x3) == 0x1) {
            uintptr_t slp = fld & 0xFFFFFC00;
            frame_free(P2V(slp));
        }
    }

    frame_free(pt);
}

/* maps addr to frame in pagetable page, null frame unmaps */
int8_t pagetable_map_page(uintptr_t vpt, uintptr_t vaddr, uintptr_t vframe)
{
    uintptr_t pt = V2P(vpt);
    uintptr_t frame = V2P(vframe);

    uintptr_t flp = 0xFFFFF000 & pt;

    uintptr_t flda = flp | ((vaddr >> 18) & 0xFFC);

    uint32_t fld = mem_read(P2V(flda));

    uintptr_t vslp;
    uintptr_t slp;

    if((fld & 0x3) != 0x1) {
        if(frame_alloc(&vslp) != 0) { return -ENOMEM; }
        slp = V2P(vslp);
        mem_set(vslp, 4096, 0);

        fld = (slp & 0xFFFFFC00) | 0x01;
        mem_write(P2V(flda), fld);
    } else {
        slp = 0xFFFFFC00 & fld;
    }

    uintptr_t slda = slp | ((vaddr >> 10) & 0xFF);

    uint32_t sld;

    if(vframe == 0) {
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
extern uint32_t blank_pagetable;

void pagetable_activate(uintptr_t pt)
{
    /* Null pagetable means unmap it all */
    if(pt == 0) {
        pt = (uintptr_t)&blank_pagetable;
    }
    /* Load the page table into TTBR0 */
    __asm volatile ("mcr p15, 0, %0, c2, c0, 0" : : "r" (V2P(pt)));

    /* Invalidate the TLB entries */
    __asm volatile ("mcr p15, 0, %0, c8, c7, 0" : : "r" (0));
}

