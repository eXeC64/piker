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

    uint32_t kstart = (uint32_t)(&_start) - 0xC0000000;
    uint32_t kend   = (uint32_t)(&_end)   - 0xC0000000;

    for(uint32_t i = kstart; i < kend; i += 0x1000) {
        frame_set(i, 1);
    }
}

uint8_t frame_get(uint32_t frame) {

    /* convert from physical address to frame index */
    frame /= 0x1000;

    if(frame > 131071) {
        return 1;
    }

    uint32_t bits = frames_bitmap[frame / 32];

    uint32_t bit = bits & (1 << (frame % 32));

    if(bit > 0) {
        return 1;
    } else {
        return 0;
    }
}

void frame_set(uint32_t frame, uint8_t status) {
    /* convert from physical address to frame index */
    frame /= 0x1000;

    /* cant set a non-existant frame */
    if(frame > 131071) {
        return;
    }

    if(status > 1) {
        status = 1;
    }

    uint32_t bits = frames_bitmap[frame / 32];

    bits &= ~(1 << (frame % 32));
    bits |= (status << (frame % 32));

    frames_bitmap[frame / 32] = bits;
}

uint32_t frame_alloc(uint32_t* frame, uint32_t num) {
    uint32_t allocated = 0;

    /* Iterate over all the frames */
    for(uint32_t i = 0; i < 4096; ++i) {
        if(frames_bitmap[i] ^ 0xFFFFFFFF) {
            /* There's a free frame here */

            /* For each bit in this part of the bitmap */
            for(uint32_t f = i * 32 * 0x1000; f < (i+1) * 32 * 0x1000; f += 0x1000) {

                /* If the frame is free */
                if(0 == frame_get(f)) {
                    frame_set(f, 1);
                    frame[allocated++] = f;

                    /* Check if we've finished allocating frames */
                    if(allocated == num) {
                        return allocated;
                    }
                }
            }
        }
    }
    /* Ran out of frames to allocate, just return how many were allocated */
    return allocated;
}

void frame_free(uint32_t frame) {
    frame_set(frame, 0);
}

