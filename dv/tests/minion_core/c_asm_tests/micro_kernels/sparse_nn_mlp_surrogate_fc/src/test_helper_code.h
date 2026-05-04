/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _TEST_HELPER_CODE_
#define _TEST_HELPER_CODE_

// Global
#include <inttypes.h>

void test_helper_code(uint32_t shire_id, uint32_t minion_id);

// Prefetches a range of code to L2
static inline void prefetch_code_l2(uint64_t start_pc, uint64_t end_pc)
{
    // Aligns
    start_pc = start_pc & ~0x3FULL;
    end_pc   = (end_pc  & ~0x3FULL) + 64;

    // Computes pending lines to prefetch
    int64_t cache_lines = (end_pc - start_pc) / 64;

    // Prefetches cache lines
    while(cache_lines > 0)
    {
        uint64_t pref_cache_lines = cache_lines - 1;
        if(pref_cache_lines > 16)
        {
            pref_cache_lines = 15;
        }
        uint64_t pref_cmd = (0x1ULL << 58)     // Dest is L2
                           | start_pc          // Prefetching starting 
                           | pref_cache_lines;

        __asm__ __volatile__ (
            "li    x31, 64\n"
            "csrw  0x81F, %[pref_cmd]\n"
          :
          : [pref_cmd] "r" (pref_cmd)
          : "x31"
        );

        // Updates pending cache lines
        cache_lines = cache_lines - (pref_cache_lines + 1);
        start_pc += 16 * 64;
    }
}

// Prefetches a range of code to L1 Icache. Mask of destination L1s
static inline void prefetch_code_l1_icache(uint64_t start_pc, uint64_t end_pc, uint64_t neigh_mask_dest)
{

}

#endif

