/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

int main()
{
    C_TEST_START;

    // enable 8 simd lanes
    __asm__ __volatile__ ( "mov.m.x m0, zero, 0xff\n");


    volatile uint32_t value = 0x3ff00000;
    volatile uint32_t * ptr = &value;

    //for(uint32_t i=0; i<(1<<23); i+=(1024+3))
    for(uint32_t i=0; i<(1<<18); i+=(1024+3))
    {
        // random sign 1b
        value = i*(1 << 13) + (et_get_rand_word()%(1 << 20));

        // Test
        __asm__ __volatile__ (
            "fbc.ps f1, 0(%[ptr])\n"
            "fcvt.ps.rast f2, f1\n"
            "frcp.ps f0, f2\n"
            "fcvt.rast.ps f2,f0\n"
            "frcp_fix.rast f0, f1, f2\n"
          : 
          : [ptr] "r" (ptr)
          : "a1", "a2", "a3", "a4"
        );
    }

    // End marker
    __asm__ __volatile__ (
            "addi x1, x1, 1\n"
            "addi x1, x1, 1\n"
            "addi x1, x1, 1\n"
            "addi x1, x1, 1\n"
            "addi x1, x1, 1\n"
            "addi x1, x1, 1\n"
            "addi x1, x1, 1\n"
      : 
      : 
      : "a7"
    );
    C_TEST_PASS
    return 0;
}
