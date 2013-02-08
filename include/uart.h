#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdarg.h>

void uart_init();
void uart_putc(uint8_t byte);
void uart_puts(const uint8_t* str);
void uart_printf(const uint8_t *format, ...);

int8_t uart_getc();
void uart_gets(uint8_t* buf, uint32_t len);

#endif /*UART_H*/
