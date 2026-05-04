/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#include "shire.h"
#include "test_compute.h"
#include "test_helper_activation.h"
#include "test_helper_weights.h"
#include "test_helper_code.h"
#include "test_helper_drain.h"
#include "test_sync.h"

// This is the entry point for the test
void test_entry_point()
{
    // Gets hart id and other ids
    uint32_t hart = get_hart_id();
    uint32_t thread_id = hart & 1;
    uint32_t minion_id = hart >> 1;
    uint32_t shire_id = minion_id >> 5;
    minion_id = minion_id & 0x1F;

    // Selects type of function based on position
    // Compute minions
    if((shire_id < 32) && (thread_id == 0))
        test_compute(shire_id, minion_id);
    else if((shire_id < 32) && (thread_id == 1) && (minion_id < 16))
        test_helper_activation(shire_id, minion_id);
    else if((shire_id < 32) && (thread_id == 1) && (minion_id < 24))
        test_helper_weights(shire_id, minion_id);
    else if((shire_id < 32) && (thread_id == 1) && (minion_id < 28))
        test_helper_drain(shire_id, minion_id);
    else if((shire_id < 32) && (thread_id == 1) && (minion_id < 29))
        test_helper_code(shire_id, minion_id);
    else if(shire_id == 32)
        test_sync(minion_id, thread_id);

    // Go to sleep in u-mode and wait for someone to provide a new PC
    __asm__ __volatile__ ("csrw   0x822, x0");
}

