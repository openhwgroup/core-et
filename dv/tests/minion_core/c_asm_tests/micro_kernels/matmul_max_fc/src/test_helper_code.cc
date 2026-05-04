/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#include "test_common.h"
#include "test_helper_code.h"
#include "test_helper_code_pass0.h"
#include "test_helper_code_pass1.h"
#include "test_helper_code_pass2.h"

void test_helper_code(uint32_t shire_id, uint32_t minion_id)
{
    __asm__ __volatile__ (
        "test_helper_code_start_point:\n"
        ".global test_helper_code_start_point\n"
    );

    ////////////////////////////////////////////////
    // First let's prefetch everything to the L2
    ////////////////////////////////////////////////

    // Sets the prefetch rate to 2 GB/sec and 12 outstanding
    // Going to LPDDR => ~400 cycles latency, 12 requests needed
    __asm__ __volatile__ (
        "li    t0, ((5 << 2) + (12 << 6))\n"
        "csrrs x0, 0x810, t0\n"
      :
      :
      : "t0"
    );

    // Waits until l2 scp is done
    fcc(FCC_1);

    // 3 passes
    test_helper_code_pass0(shire_id, minion_id);
#ifndef SINGLE_PASS
    test_helper_code_pass1(shire_id, minion_id);
    // Waits to finish 1st layer to start prefetch of 3rd
    // always 1 layer ahead
    fcc(FCC_1);
    test_helper_code_pass2(shire_id, minion_id);
#endif

    // End code marker
    __asm__ __volatile__ (
        "test_helper_code_end_point:\n"
        ".global test_helper_code_end_point\n"
    );
}

