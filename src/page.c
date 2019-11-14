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
    /* take MSB 20 bits of page table address */
    uintptr_t flp = 0xFFFFF000 & V2P((uintptr_t)pt);

    /* combine with MSB 10 bits of virtual address to produce
     * first level descriptor address. LSB 2 bits are 0.
     */
    uintptr_t flda = flp | (((uint32_t)virt >> 18) & 0xFFC);

    /* read the first level descriptor. Each fld controls 1MB
     * i.e. 0x0000 0000 - 0x000f ffff*/
    uint32_t fld = mem_read(P2V(flda));


    /* fld[1:0] == 0 means unmapped
     * fld[1:0] == 1 means entry is physical address of
     *               coarse second level descriptor. Coarse tables are 1KB
     *               and control 1MB of address space.
     * fld[1:0] == 2 means section descriptor
     * fld[1:0] == 3 means entry is physical address of
     *               fine second level descriptor
     */

    uintptr_t vslp;
    uintptr_t slp;

    if ((fld & 0x3) != 0x1) {
        /* allocate a new second level descriptor */
        if (frame_alloc((void**)&vslp) != 0) { return -ENOMEM; }
        slp = V2P(vslp);
        mem_set(vslp, 4096, 0);

        /* and write its address to the first level descriptor */
        fld = (slp & 0xFFFFFC00) | 0x01;
        mem_write(P2V(flda), fld);
    } else {
        /* fetch the address of the existing second level descriptor */
        slp = 0xFFFFFC00 & fld;
    }

    /* MSB 22 bits of fld with virt[19:12] with 2 LSB bits of 0 gives address
     * of second level descriptor */
    uintptr_t slda = slp | (((uintptr_t)virt >> 10) & 0xFF);

    /* construct a second level descriptor.
     * Each 32-bit descriptor manages 4K of memory (1 frame)
     * sld[31:12] = frame address
     * sld[11:4] = access permissions
     * sld[3] = cacheable
     * sld[2] = bufferable
     * sld[1:0] =  2 (indicates small frame)
     */
    uint32_t sld;
    if (phy == 0) {
        /* 0 target address means unmap */
        sld = 0;
    } else {
        sld = (uintptr_t)phy | 0x72;
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

