/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

int main()
{
    C_TEST_START;

    // enable 8 simd lanes
    __asm__ __volatile__ ( "mov.m.x m0, zero, 0xff\n");

    volatile int value = 0x00000000;
    volatile int * ptr = &value;

    for(int i=(1<<15); i<(1<<23); i+=(1024+1))
    {
        // random sign 1b
        value = (et_get_rand_word() % 2) << 31;

        // random exponent 8b (only Ex<0 since we accept only fractional part, integer part does not contribute)
        volatile int exponent = et_get_rand_word() % 128;
        value += (exponent == 0  ) ?        1 << 23 :  // no support for denormalized
                                     exponent << 23;
                                  
        // sweep mantissa 23b
        value += i;

        // Test
        __asm__ __volatile__ (
            "fbc.ps f1, 0(%[ptr])\n"
            // RTLMIN-5479"frsq.ps f0, f1\n"
          : 
          : [ptr] "r" (ptr)
          : "a1", "a2", "a3", "a4"
        );
    }

    C_TEST_PASS;
    return 0;
}
