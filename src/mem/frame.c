#include "kernel.h"

#include "mem/frame.h"

/* frames: 131072
 * frames / 32 : 4096
 *
 * frame 0 is 0x0 phys, frame 1 is 0x1000 phys, etc.
 */

uint32_t frames_bitmap[4096] __attribute__((aligned (4096)));

void frame_init() {
    /* All frames are free to begin with */
    for(uint32_t i = 0; i < 4096; ++i) {
        frames_bitmap[i] = 0x00000000;
    }

    /* Now lets mark the kernel's frames as taken */
    uintptr_t kstart = (uintptr_t)(&_start) - 0xC0000000;
    uintptr_t kend   = (uintptr_t)(&_end)   - 0xC0000000;

    for(uintptr_t i = kstart; i < kend; i += 0x1000) {
        frame_set(i, 1);
    }

    /* We also need to mark the GPU's frames as taken */
    /* For now we're assuming we have 512MB of memory
     * and 64 MB of it is allocated to the GPU */

    /* 64 MB */
    uintptr_t gpu_size = 0x04000000;
    uintptr_t gpu_end =  0x20000000;
    uintptr_t gpu_start =  gpu_end - gpu_size;

    for(uintptr_t i = gpu_start; i < gpu_end; i += 0x1000) {
        frame_set(i, 1);
    }
}

uint8_t frame_get(uintptr_t frame) {
    /* convert from physical address to frame index */
    uint32_t fi = frame / 0x1000;

    if(fi > 131071) {
        return 1;
    }

    uint32_t bits = frames_bitmap[fi / 32];

    uint32_t bit = bits & (1 << (fi % 32));

    if(bit > 0) {
        return 1;
    } else {
        return 0;
    }
}

void frame_set(uintptr_t frame, uint8_t status) {
    /* convert from physical address to frame index */
    uint32_t fi = frame / 0x1000;

    /* cant set a non-existant frame */
    if(fi > 131071) {
        return;
    }

    if(status > 1) {
        status = 1;
    }

    uint32_t bits = frames_bitmap[fi / 32];

    bits &= ~(1 << (fi % 32));
    bits |= (status << (fi % 32));

    frames_bitmap[fi / 32] = bits;
}

uint32_t frame_alloc(uintptr_t* frame) {
    /* Iterate over all the frames */
    for(uint32_t i = 0; i < 4096; ++i) {
        if(frames_bitmap[i] ^ 0xFFFFFFFF) {
            /* There's a free frame here */

            /* For each bit in this part of the bitmap */
            for(uintptr_t f = i * 32 * 0x1000; f < (i+1) * 32 * 0x1000; f += 0x1000) {

                /* If the frame is free */
                if(0 == frame_get(f)) {
                    frame_set(f, 1);
                    *frame = f + 0xC0000000; /* virt address */
                    return 1;
                }
            }
        }
    }
    return 0;
}

uint32_t frame_alloc_mult(uintptr_t* frames, size_t num) {
    for(uint32_t i = 0; i < num; ++i) {
        uint32_t result = frame_alloc(&(frames[i]));
        if(result == 0) {
            /* Allocation failed, we're out of frames */
            return i;
        }
    }

    return num;
}

uint32_t frame_alloc_mult_contig(uintptr_t* frames, size_t num) {
    /* Iterate over all the frames */
    for(uint32_t i = 0; i < 131072; ++i) {
        if(frame_get(i * 0x1000) == 0) {
            uint8_t enough = TRUE;

            for(uint32_t j = 0; j < num; ++j) {
                if(frame_get(i + j * 0x1000) == 1) {
                    enough = FALSE;
                    break;
                }
            }

            if(enough == TRUE) {
                for(uint32_t j = 0; j < num; ++j) {
                    frame_set(i + j * 0x1000, 1);
                    frames[j] = i + j * 0x1000 + 0xC0000000;
                }
                return num;
            }
        }
    }
    return 0;
}

void frame_free(uintptr_t frame) {
    frame_set(frame - 0xC0000000, 0); /* convert from virt address */
}

