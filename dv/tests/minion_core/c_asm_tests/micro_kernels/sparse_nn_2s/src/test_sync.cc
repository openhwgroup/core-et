/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#include "test_sync.h"
#include "test_common.h"
#include "utils.h"

void test_sync(uint32_t minion_id, uint32_t thread_id)
{
    // Disable all sync threads for shires that are not going to do anything
    if((((minion_id * 2) + thread_id) - 32) >= SIM_SHIRES)
        return;

    // The first global barrier is related to L2 Scp init
    global_barrier_receiver(
        0,           // Which FCC to wait for
        0,           // FLB to be used in the source shire for the barrier
        minion_id,   // Sync minion id
        thread_id,   // Sync thread id
        0,           // Thread of the FCC dest
        0,           // FCC for dest
        0xFFFFFFFF); // Mask of minions in dest shire to receive FCC
}

