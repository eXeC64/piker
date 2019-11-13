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

void frame_init(void);

int frame_get(void *frame);
int frame_set(void *frame, int status);

int frame_alloc(void **frame);
void frame_free(void *frame);

int frame_alloc_aligned(void **frame, unsigned long alignment);
size_t frame_alloc_mult(void **frames, size_t num);
size_t frame_alloc_mult_contig(void **frames, size_t num);

#endif /*FRAME_H*/
