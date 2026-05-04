/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#include "test_sync.h"
#include "test_common.h"

void test_sync(uint32_t minion_id, uint32_t thread_id)
{
    if (((minion_id * 2 + thread_id) - 32) >= N_SHIRES_COMPUTE)
      return;

    // The first global barrier is related to L2 Scp init
    global_barrier_receiver(
        FCC_0,            // Which FCC to wait for
        0,                // FLB to be used in the source shire for the barrier
        minion_id,        // Sync minion id
        thread_id,        // Sync thread id
        THREAD_0,         // Thread of the FCC dest
        FCC_0,            // FCC for dest
        COMPUTE_THREADS); // Mask of minions in dest shire to receive FCC

    // 3 layers
#ifndef SINGLE_PASS
    for(int i = 0; i < 3; i++)
    {
        global_barrier_receiver(
            FCC_0,                    // Which FCC to wait for
            0,                        // FLB to be used in the source shire for the barrier
            minion_id,                // Sync minion id
            thread_id,                // Sync thread id
            THREAD_1,                 // Thread of the FCC dest
            FCC_1,                    // FCC for dest
            HELPER_ACTIVATION_THREADS // Mask of minions in dest shire to receive FCC
          | HELPER_WEIGHTS_THREADS
          | HELPER_CODE_THREADS);
    }
#else
        global_barrier_receiver(
            FCC_0,                    // Which FCC to wait for
            0,                        // FLB to be used in the source shire for the barrier
            minion_id,                // Sync minion id
            thread_id,                // Sync thread id
            THREAD_1,                 // Thread of the FCC dest
            FCC_1,                    // FCC for dest
            HELPER_ACTIVATION_THREADS // Mask of minions in dest shire to receive FCC
          | HELPER_WEIGHTS_THREADS
          | HELPER_CODE_THREADS);
#endif

}

