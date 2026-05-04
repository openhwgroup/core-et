/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Global
#include <inttypes.h>

// Local
#include "shire.h"

volatile uint64_t global_array[4096]__attribute__((aligned(4096)));
volatile uint64_t barrier[8] __attribute__((aligned(4096))) = { 0, 0, 0, 0, 0, 0, 0, 0 };

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
        uint64_t barrier_new_value = 1;
        __asm__ __volatile__(
            "amoswapg.d x0, %[barrier_new_value], (%[barrier])\n"
            :
            : [global_array] "r" (global_array),
              [barrier] "r" (barrier),
              [barrier_new_value] "r" (barrier_new_value)
            : "memory"
        );
    }
    if((shire_id == 1) && (minion_id == 0) && (thread_id == 0))
    {
        // Waits for other thread to finish
        uint64_t barrier_value = 0;
        while(barrier_value == 0)
        {
            __asm__ __volatile__(
                "amoorg.d %[barrier_value], x0, (%[barrier])\n"
                : [barrier_value] "+&r" (barrier_value)
                : [global_array] "r" (global_array),
                  [barrier] "r" (barrier)
                : "memory"
            );
        }
        // L1 write
        global_array[0] = 0;
    }
}

