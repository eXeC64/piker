#include "kernel.h"

#include "uart.h"
#include "timer.h"
#include "mem/frame.h"
#include "framebuffer.h"
#include "mailbox.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    uart_init();
    uart_puts("Piker 0.0.0\n");

    frame_init();
    mem_alloc_init();

    uint32_t fb_ptr;
    frame_alloc(&fb_ptr);

    framebuffer_request_t* fb = (framebuffer_request_t*) fb_ptr;

    fb->width = 800;
    fb->height = 600;
    fb->virtual_width = 800;
    fb->virtual_height = 600;
    fb->pitch = 0;
    fb->depth = 32;
    fb->x_offset = 0;
    fb->y_offset = 0;
    fb->pointer = NULL;
    fb->size = 0;

    mailbox_send(fb_ptr, 0x1);

    mailbox_read(0x1);

    while(fb->pointer == NULL) {
        uart_printf("pointer still null at %ims\n", (uint32_t)(timer_now() / 1000));
        timer_sleep(1000);
    }

    uart_printf("w,h: %i,%i\n", fb->width, fb->height);
    uart_printf("size: %i\n", fb->size);
    uart_printf("depth: %i\n", fb->depth);

    fb->pointer += 0x80000000;

    for(uint32_t i = 0; i < fb->size; ++i) {
        uint32_t* pixel = (uint32_t*) fb->pointer + i / 4;
        *pixel = 0xFF0000FF;
    }

    /* y * pitch + x * 3 + rgb_channel */

    while(TRUE) {
        uart_printf("time: %ims\n", (uint32_t)(timer_now() / 1000));
        timer_sleep(1000);
    }

    uart_puts("\n*** HALTING ***\n");
}
