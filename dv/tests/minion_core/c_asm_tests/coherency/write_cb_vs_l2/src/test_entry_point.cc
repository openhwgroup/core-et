/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Global
#include <inttypes.h>

// Local
#include "shire.h"
#include "utils.h"

volatile uint64_t global_array[4096]__attribute__((aligned(4096)));

// This is the entry point for the test
void test_entry_point()
{
    // Gets hart id and other ids
    uint32_t hart = get_hart_id();
    uint32_t thread_id = hart & 1;
    uint32_t minion_id = hart >> 1;
    uint32_t shire_id = minion_id >> 5;
    minion_id = minion_id & 0x1F;

    if((shire_id == 0) && (minion_id == 0) && (thread_id == 0))
    {
        // L2 write
        __asm__ __volatile__(
            "fswl.ps  f0, 0(%[global_array])\n"
            :
            : [global_array] "r" (global_array)
            : "memory"
        );
        // CB write
        uint64_t tstore = (uint64_t) global_array;
        __asm__ __volatile__(
	          "csrw     0x87F, %[tstore]\n"
            :
            : [tstore] "r" (tstore)
            : "memory"
        );
    }
}

