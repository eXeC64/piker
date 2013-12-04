#ifndef STRING_H
#define STRING_H

#include "types.h"

size_t strlen(const uint8_t* str);

void int_to_str(int64_t value, uint64_t radix, uint8_t* buf);

#endif /*STRING_H*/
