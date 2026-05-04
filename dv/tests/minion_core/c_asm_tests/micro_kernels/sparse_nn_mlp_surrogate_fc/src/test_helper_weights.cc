/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#include "test_helper_weights.h"
#include "test_helper_weights_pass0.h"
#include "test_helper_weights_pass1.h"
#include "test_helper_weights_pass2.h"
#include "test_common.h"

void test_helper_weights(uint32_t shire_id, uint32_t minion_id)
{
     __asm__ __volatile__ (
        "test_helper_weights_start_point:\n"
        ".global test_helper_weights_start_point\n"
    );

    // All 8 minions prefetching
    //if(minion_id >= 20)
    //    return;

    // Sets the prefetch rate to 1 GB/sec (total of 8 GB/sec) and 6 outstanding
    // Weights are in DDR. 8 GB/sec/shire * 32 shires = 256 GB/sec, but shires
    // are requesting same data in pairs of two, so actual DDR request is 128 GB/sec
    // which is maximum
    __asm__ __volatile__ (
        "li    t0, ((6 << 2) + (6 << 6))\n"
        "csrrs x0, 0x810, t0\n"
      :
      :
      : "t0"
    );

    // Weight threads can be decoupled up to 4 iterations
    // Use FCC0 for this purpose. As 8 threads, first 4 gives one credit
    if(minion_id < 20)
    {
        volatile uint64_t * fcc_to_o_t_addr = (uint64_t *) IPI_THREAD1;
        uint64_t fcc_to_o_t_mask = HELPER_WEIGHTS_THREADS;

        * fcc_to_o_t_addr = fcc_to_o_t_mask;
    }

    // Waits until l2 scp is done
    fcc(FCC_1);

    // 3 passes
    test_helper_weights_pass0(shire_id, minion_id);
#ifndef SINGLE_PASS
    test_helper_weights_pass1(shire_id, minion_id);
    //test_helper_weights_pass2(shire_id, minion_id);
#endif

     __asm__ __volatile__ (
        "test_helper_weights_end_point:\n"
        ".global test_helper_weights_end_point\n"
    );
}

