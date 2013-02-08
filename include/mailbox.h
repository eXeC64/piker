#ifndef MAILBOX_H
#define MAILBOX_H

#include <stdint.h>

void mailbox_send(uint32_t data, uint32_t channel);
uint32_t mailbox_read(uint32_t channel);

#endif /*MAILBOX_H*/
