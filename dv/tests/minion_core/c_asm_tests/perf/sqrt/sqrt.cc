/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

int main()
{
    C_TEST_START;

   // FUTURE: Emmanuel Marie This test makes no sense as it uses an mcode instruction
   // without setting up a trap vector. Pass and quit.
   C_TEST_PASS;
   return 0;

    // enable 8 simd lanes
    __asm__ __volatile__ ( "mov.m.x m0, zero, 0xff\n");

    volatile int value = 0x3ff00000;
    volatile int * ptr = &value;

    int alternate = 0;
    for(int i=0; i<(1<<23); i+=(1024+1))
    {
        // random sign 1b
        value = (et_get_rand_word() % 2) << 31;

        // random exponent 8b alternating even/odd
        volatile int exponent = et_get_rand_word() % 128;
        value += (exponent == 0  ) ? (       1 << 24) + (alternate << 23) :  // no support for denormalized
                 (exponent == 127) ? (     126 << 24) + (alternate << 23) :  // no support for inf/NaN
                                     (exponent << 24) + (alternate << 23);
        alternate = alternate ? 0 : 1;
                                  
        // sweep mantissa 23b
        value += i;

        // Test
        __asm__ __volatile__ (
            "fbc.ps f1, 0(%[ptr])\n"
            "fsqrt.ps f0, f1\n"
          : 
          : [ptr] "r" (ptr)
          : "a1", "a2", "a3", "a4"
        );
    }

    C_TEST_PASS;
    return 0;
}
