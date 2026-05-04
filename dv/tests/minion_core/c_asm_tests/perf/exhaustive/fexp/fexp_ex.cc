/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

inline void check(unsigned int sign, unsigned int exponent, unsigned int fraction){
    // random sign 1b
    volatile unsigned int value = (sign << 31) + ((exponent % (1 << 8)) << 23) + (fraction % (1 << 23));

    // Test
    __asm__ __volatile__ (
            "fbc.ps f1, 0(%[ptr])\n"
            "fexp.ps f0, f1\n"
            : 
            : [ptr] "r" (&value)
            : "a1", "a2", "a3", "a4"
            );
}

int main()
{
    C_TEST_START;

    // enable 8 simd lanes
    __asm__ __volatile__ ( "mov.m.x m0, zero, 0xff\n");

    //for(int i=0; i<(1<<23); i+=(512+1))
    //for(int i=0; i<(1<<10); i+=(512+1))
    for(uint32_t exponent=1; exponent < ((1 << 8)-1); ++exponent)
        for(uint32_t fraction=0; fraction < (1 << 22); ++fraction){
            check(0, exponent, fraction);
            check(1, exponent, fraction);
        }

    check(0, 0, 0);
    check(1, 0, 0);

    C_TEST_PASS;
    return 0;
}
