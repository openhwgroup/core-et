/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "pmu_not_used.h"

inline __attribute__((naked)) void pmu_mtrap_vector(void)
{
   __asm__ __volatile__ (
         ".align 12\n\t"
         "mtrap_vector%=: csrr s1, mtval\n\t"
         "csrr s2, mcause\n\t"
         "csrr s3, mepc\n\t"
         "addi s3, s3, 4\n\t"
         "csrw mepc, s3\n\t"
         "mret\n\t"
         :::
   );
}

// Setting up mtrap vector
inline void setup_pmu_mtrap_vector(void)
{
    void (*mtvec_ptr)(void) = &pmu_mtrap_vector;
    __asm__ __volatile__ (
        "csrw mtval, t0\n\t"
        "csrw stval, t0\n\t"
        "mv t0, %[mtvec]\n\t"
        "csrw mtvec, t0\n\t"
        :
        :[mtvec] "r" (mtvec_ptr)
        :
  );
}


int main() {
    C_TEST_START

    setup_pmu_mtrap_vector();
    
    // test hpmcounter
    for (uint32_t i = 9; i < 32; ++i) {
        read_hpmcounter(i);
        //The CPU should trap and skip C_TEST_FAIL
        //C_TEST_FAIL
    }

    // test mhpmcounter
    for (uint32_t i = 9; i < 32; ++i) {
        uint64_t read_value = read_mhpmcounter(i);
        et_printf_long("[TEST]: Value of mhpmcounter[%u] = %d\n", i, read_value);        
        if (read_value != 0) {
            C_TEST_FAIL;
        }
    }

    // test mhpmevent
    for (uint32_t i = 9; i < 32; ++i) {
        uint64_t read_value = read_mhpmevent(i);
        et_printf_long("[TEST]: Value of mhpmevent[%u] = %d\n", i, read_value);
        if (read_value != 0) {
            C_TEST_FAIL;
        }
    }

    C_TEST_PASS
}
