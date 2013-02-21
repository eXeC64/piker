#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>

/*
 *  Frames are 4kb chunks of physical memory.
 *  The raspberry pi has up to 512MB of it
 *  currently. Well, at least 32MB of it is
 *  for the GPU.
 *
 *  This is for the allocation of that
 *  physical memory. We'll give it out in
 *  4kb "frames".
 *
 *  The frames are kept track of in a bitmap.
 *
 *  There are 131072 frames, (512MiB/4KiB)
 */

void frame_init();

uint8_t frame_get(uint32_t frame);
void frame_set(uint32_t frame, uint8_t status);

uint32_t frame_alloc(uint32_t* frame);
uint32_t frame_alloc_mult(uint32_t* frames, uint32_t num);
uint32_t frame_alloc_mult_contig(uint32_t* frames, uint32_t num);
void frame_free(uint32_t frame);

#endif /*FRAME_H*/
