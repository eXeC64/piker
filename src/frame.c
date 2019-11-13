#include "kernel.h"

#include "frame.h"

/* frames: 131072
 * frames / 32 : 4096
 *
 * frame 0 is 0x0 phys, frame 1 is 0x1000 phys, etc.
 */

uint32_t frames_bitmap[4096] __attribute__((aligned (4096)));

void frame_init(void)
{
    /* All frames are free to begin with */
    for (int i = 0; i < 4096; ++i) {
        frames_bitmap[i] = 0x00000000;
    }

    /* Now lets mark the kernel's frames as taken */
    void *kstart = V2P(&_start);
    void *kend   = V2P(&_end);

    for (char *i = kstart; i < (char*)kend; i += 0x1000) {
        frame_set(i, 1);
    }

    /* We also need to mark the GPU's frames as taken */
    /* For now we're assuming we have 512MB of memory
     * and 64 MB of it is allocated to the GPU */

    /* 64 MB */
    void *gpu_size = (void*)0x04000000;
    void *gpu_end =  (void*)0x20000000;
    void *gpu_start = (void*)(gpu_end - gpu_size);

    for (char *i = gpu_start; i < (char*)gpu_end; i += 0x1000) {
        frame_set(i, 1);
    }

    /* Mark frame 0 as taken, so null pointers *NEVER* point to anything */
    frame_set(0, 1);
}

int frame_get(void *frame)
{
    /* convert from physical address to frame index */
    uint32_t fi = (uint32_t)(frame) / 0x1000;

    if (fi > 131071) {
        return 1;
    }

    uint32_t bits = frames_bitmap[fi / 32];

    uint32_t bit = bits & (1 << (fi % 32));

    if (bit > 0) {
        return 1;
    } else {
        return 0;
    }
}

int frame_set(void *frame, int status)
{
    /* convert from physical address to frame index */
    uint32_t fi = (uint32_t)(frame) / 0x1000;

    /* cant set a non-existant frame */
    if (fi > 131071) {
        return -EINVAL;
    }

    if (status) {
        status = 1;
    }

    uint32_t bits = frames_bitmap[fi / 32];

    bits &= ~(1 << (fi % 32));
    bits |= (status << (fi % 32));

    frames_bitmap[fi / 32] = bits;

    return 0;
}

int frame_alloc(void **frame)
{
    return frame_alloc_aligned(frame, 0x1000);
}

int frame_alloc_aligned(void **frame, uint32_t alignment)
{
    uint32_t mask = alignment - 1;

    /* Iterate over all the frames */
    for (uint32_t i = 0; i < 4096; ++i) {
        if (frames_bitmap[i] ^ 0xFFFFFFFF) {
            /* There's a free frame here */

            /* For each bit in this part of the bitmap */
            for (uintptr_t f = i * 32 * 0x1000; f < (i+1) * 32 * 0x1000; f += 0x1000) {

                /* If the frame is free */
                if (0 == frame_get((void*)f) && (f & mask) == 0) {
                    frame_set((void*)f, 1);
                    *frame = (void*)P2V(f);
                    return 0;
                }
            }
        }
    }
    return -ENOMEM;
}

size_t frame_alloc_mult(void **frames, size_t num)
{
    for(uint32_t i = 0; i < num; ++i) {
        uint32_t result = frame_alloc(&(frames[i]));
        if(result == 0) {
            /* Allocation failed, we're out of frames */
            return i;
        }
    }

    return num;
}

size_t frame_alloc_mult_contig(void **frames, size_t num)
{
    /* Iterate over all the frames */
    for(uint32_t i = 0; i < 131072; ++i) {
        if(frame_get((void*)(i * 0x1000)) == 0) {
            uint8_t enough = TRUE;

            for(uint32_t j = 0; j < num; ++j) {
                if(frame_get((void*)(i + j * 0x1000)) == 1) {
                    enough = FALSE;
                    break;
                }
            }

            if(enough == TRUE) {
                for(uint32_t j = 0; j < num; ++j) {
                    frame_set((void*)(i + j * 0x1000), 1);
                    frames[j] = (void*)P2V(i + j * 0x1000);
                }
                return num;
            }
        }
    }
    return 0;
}

void frame_free(void *frame)
{
    frame_set(V2P(frame), 0); /* convert from virt address */
}

