#ifndef KERNEL_H
#define KERNEL_H

/* Utility header, included at top of all source files */

/* External headers */
#include <stdint.h>
#include <stdarg.h>

/* Commonly used headers */
#include "mem/mmio.h"

/* Global constants */
#define NULL 0
#define TRUE 1
#define FALSE 0

/* Global macros */
#define STR(x) (const uint8_t*)(x)
/* Conversion between physical and bus addresses */
#define PHYS_TO_BUS(x) (void*)((uint32_t)(x) + 0xC0000000)
#define BUS_TO_PHYS(x) (void*)((uint32_t)(x) - 0xC0000000)

/* Address Table */
enum {
    IO_BASE         = 0x20000000,
 
    SYS_TIMER_BASE  = (IO_BASE + 0x00003000),
    SYS_TIMER_CS    = (SYS_TIMER_BASE + 0x00),
    SYS_TIMER_CLO   = (SYS_TIMER_BASE + 0x04),
    SYS_TIMER_CHI   = (SYS_TIMER_BASE + 0x08),
    SYS_TIMER_C0    = (SYS_TIMER_BASE + 0x0C),
    SYS_TIMER_C1    = (SYS_TIMER_BASE + 0x10),
    SYS_TIMER_C2    = (SYS_TIMER_BASE + 0x14),
    SYS_TIMER_C3    = (SYS_TIMER_BASE + 0x18),

    MAILBOX_BASE    = (IO_BASE + 0x0000B880),
    MAILBOX_READ    = (MAILBOX_BASE + 0x00),
    MAILBOX_PEAK    = (MAILBOX_BASE + 0x10),
    MAILBOX_SENDER  = (MAILBOX_BASE + 0x14),
    MAILBOX_STATUS  = (MAILBOX_BASE + 0x18),
    MAILBOX_CONFIG  = (MAILBOX_BASE + 0x1C),
    MAILBOX_WRITE   = (MAILBOX_BASE + 0x20),

    GPIO_BASE       = (IO_BASE + 0x00200000),
    GPPUD           = (GPIO_BASE + 0x94),
    GPPUDCLK0       = (GPIO_BASE + 0x98),

    UART0_BASE      = (IO_BASE + 0x00201000),
    UART0_DR        = (UART0_BASE + 0x00),
    UART0_RSRECR    = (UART0_BASE + 0x04),
    UART0_FR        = (UART0_BASE + 0x18),
    UART0_ILPR      = (UART0_BASE + 0x20),
    UART0_IBRD      = (UART0_BASE + 0x24),
    UART0_FBRD      = (UART0_BASE + 0x28),
    UART0_LCRH      = (UART0_BASE + 0x2C),
    UART0_CR        = (UART0_BASE + 0x30),
    UART0_IFLS      = (UART0_BASE + 0x34),
    UART0_IMSC      = (UART0_BASE + 0x38),
    UART0_RIS       = (UART0_BASE + 0x3C),
    UART0_MIS       = (UART0_BASE + 0x40),
    UART0_ICR       = (UART0_BASE + 0x44),
    UART0_DMACR     = (UART0_BASE + 0x48),
    UART0_ITCR      = (UART0_BASE + 0x80),
    UART0_ITIP      = (UART0_BASE + 0x84),
    UART0_ITOP      = (UART0_BASE + 0x88),
    UART0_TDR       = (UART0_BASE + 0x8C),
};

#endif /*KERNEL_H*/
