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

    // Only 4 minions prefetching
    if (minion_id >= 20)
        return;

    // Sets the prefetch rate to 2 GB/sec (total of 8 GB/sec) and 12 outstanding
    __asm__ __volatile__ (
        "li    t0, ((5 << 2) + (12 << 6))\n"
        "csrrs x0, 0x810, t0\n"
      :
      :
      : "t0"
    );

    // Send several FCC1 credits to compute minions ahead of time (4 minions x 32 = 128)
    // Typical case weight will be already be present
    for(uint64_t i = 0; i < 32; i++)
        fcc_send(shire_id, COMPUTE_THREADS, THREAD_0, FCC_1);

    // Waits until l2 scp is done
    fcc(FCC_1);

    // Weight threads can be decoupled up to 4 iterations
    // Use FCC0 for this purpose. As 4 threads, each 1 gives one credit
    volatile uint64_t * fcc_to_o_t_addr = (uint64_t *) IPI_THREAD1;
    uint64_t fcc_to_o_t_mask = HELPER_WEIGHTS_THREADS;

    * fcc_to_o_t_addr = fcc_to_o_t_mask;

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

