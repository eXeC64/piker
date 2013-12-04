#include "kernel.h"

#include "uart.h"
#include "string.h"
#include "timer.h"

void uart_init() {
    mem_write(UART0_CR, 0x00000000);
    mem_write(GPPUD,0x00000000);
    timer_sleep(1);
 
    mem_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    timer_sleep(1);
 
    mem_write(GPPUDCLK0, 0x00000000);
    mem_write(UART0_ICR, 0x7FF);
    mem_write(UART0_IBRD, 1);
    mem_write(UART0_FBRD, 40);
 
    mem_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
    mem_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) |
		    (1 << 6) | (1 << 7) | (1 << 8) |
		    (1 << 9) | (1 << 10));
    mem_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}
 
void uart_putc(uint8_t byte) {
    while(mem_read(UART0_FR) & (1 << 5)) { ; }
    mem_write(UART0_DR, byte);
}

void uart_puts(const uint8_t *str) {
    while (*str) {
        uart_putc(*str++);
    }
}

void uart_printf(const uint8_t *format, ...) {
    va_list args;
    va_start(args, format);

    uint8_t buf[128];

    const uint8_t* c = format;
    for(; *c != 0; ++c) {
        if(*c == '%') {
            uint8_t f = *(c+1);

            switch(f) {
                case '%':
                    uart_putc('%');
                    c++;
                    break;
                case 's':
                    uart_puts( va_arg(args, uint8_t*) );
                    c++;
                    break;
                case 'i':
                    int_to_str( va_arg(args, uint32_t), 10, buf);
                    uart_puts(buf);
                    c++;
                    break;
                case 'n':
                    int_to_str( va_arg(args, int32_t), 10, buf);
                    uart_puts(buf);
                    c++;
                    break;
                case 'x':
                    int_to_str( va_arg(args, uint32_t), 16, buf);
                    uart_puts(buf);
                    c++;
                    break;
                case 'b':
                    int_to_str( va_arg(args, uint32_t), 2, buf);
                    uart_puts(buf);
                    c++;
                    break;
                case 'c':
                    uart_putc(va_arg(args, uint32_t));
                    c++;
                    break;
            }
        } else {
            uart_putc(*c);
        }
    }
}

int8_t uart_getc() {
    while (1) {
        if (!(mem_read(UART0_FR) & (1 << 4))) {
	    break;
	}
    }
    return mem_read(UART0_DR);
}

void uart_gets(uint8_t* buf, size_t len) {
    size_t recv = 0;
    while(recv < len - 1) {
        uint8_t c = uart_getc();

        switch(c) {
            /* end of line */
            case '\n':
                buf[recv] = '\0';
                uart_putc('\n');
                return;

            /* backspace */
            case 0x08:
                uart_putc(c);
            case 0x7f:
                --recv;
                break;

            /* other characters */
            default:
                uart_putc(c);
                buf[recv++] = c;
        }

    }
    buf[len-1] = '\0';
    uart_putc('\n');
}
