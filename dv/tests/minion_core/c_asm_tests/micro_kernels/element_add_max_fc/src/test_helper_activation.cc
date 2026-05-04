/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#include "test_helper_activation.h"
#include "test_helper_activation_pass0.h"
#include "test_helper_activation_pass1.h"
#include "test_helper_activation_pass2.h"
#include "esr.h"
#include "test_common.h"

void test_helper_activation(uint32_t shire_id, uint32_t minion_id)
{
	 __asm__ __volatile__ (
	    "test_helper_activation_start_point:\n"
    	".global test_helper_activation_start_point\n"
	);

    // Only 8 minions prefetching
    // Enabling every other minion to distribute as much as possible the request
    // bandwidth between two neighs
    if (minion_id & 1)
        return;

    minion_id = minion_id >> 1;

    // Sets the prefetch rate to 4 GB/sec (total of 32 GB/sec) and 16 outstanding
    __asm__ __volatile__ (
        "li    t0, ((4 << 2) + (16 << 6))\n"
        "csrrs x0, 0x810, t0\n"
      :
      :
      : "t0"
    );

    // 3 passes
    test_helper_activation_pass0(shire_id, minion_id);
#ifndef SINGLE_PASS
    test_helper_activation_pass1(shire_id, minion_id);
    test_helper_activation_pass2(shire_id, minion_id);
#endif

    // Waits for credits to come back to sync going to firmware at end of test
    // with compute minions
    fcc(0);
    fcc(0);
    fcc(0);
    fcc(0);
    fcc(0);
    __asm__ __volatile__ (
	    "test_helper_activation_end_point:\n"
    	".global test_helper_activation_end_point\n"
	);
}
