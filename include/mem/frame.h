#ifndef FRAME_H
#define FRAME_H

#include "types.h"

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

int8_t frame_init();

int8_t frame_get(uintptr_t frame);
int8_t frame_set(uintptr_t frame, uint8_t status);

int8_t frame_alloc(uintptr_t* frame);
int8_t frame_free(uintptr_t frame);

size_t frame_alloc_mult(uintptr_t* frames, size_t num);
size_t frame_alloc_mult_contig(uintptr_t* frames, size_t num);

#endif /*FRAME_H*/
