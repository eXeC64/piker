#include "kernel.h"

#include "string.h"

uint32_t strlen(const uint8_t* str) {
    uint32_t len;
    for(len = 0; str[len] != 0; len++) {;}
    return len;
}

void int_to_str(int64_t value, uint64_t radix, uint8_t* buf) {
   uint8_t tmp[128];
   uint8_t* tp = tmp;
   int64_t i;
   int64_t sign;
   uint64_t v;

   sign = (radix == 10 && value < 0);
   if(sign) {
       v = -value;
   } else {
       v = (uint64_t) value;
   }

   while( v || tp == tmp) {

       i = v % radix;

       v /= radix;

       if( i < 10) {
           *tp++ = i+'0';
       } else {
           *tp++ = i+'a' - 10;
       }
   }

   if(sign) {
       *buf++ = '-';
   }

   while(tp>tmp) {
       *buf++ = *--tp;
   }

   *buf = '\0';

}

