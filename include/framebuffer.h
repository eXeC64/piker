#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "kernel.h"

typedef volatile struct {
    uint32_t width;
    uint32_t height;
    uint32_t virtual_width;
    uint32_t virtual_height;
    uint32_t pitch;
    uint32_t depth;
    uint32_t x_offset;
    uint32_t y_offset;
    uint32_t pointer;
    uint32_t size;
} framebuffer_request_t;

#endif /*FRAMEBUFFER_H*/
