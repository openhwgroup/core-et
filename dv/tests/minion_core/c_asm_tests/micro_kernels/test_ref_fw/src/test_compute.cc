/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#include "test_compute.h"
#include "esr.h"

void test_compute(uint32_t shire_id, uint32_t minion_id)
{
    // Barrier to send a ready message to master shire (fcc1)
    uint64_t fcc_to_master = FCC0_MASTER; 
    __asm__ __volatile__
    (
        "li     t0, 992\n"                  // Do a barrier on #0 with 32 threads (compute only)
        "csrrw  t0, 0x820, t0\n"
        "beq    t0, x0, 1f\n"
        "li     t0, 1\n"                    // FCC to master minion only
        "sd     t0, 0(%[fcc_to_master])\n"
        "1:\n"
        "csrw   0x822, x0\n"                // Go to sleep
      :
      : [fcc_to_master] "r" (fcc_to_master)
      : "t0"
    );

}

