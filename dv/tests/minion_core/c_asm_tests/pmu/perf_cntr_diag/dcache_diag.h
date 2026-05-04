/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "cacheops.h"

#ifndef ALWAYS_INLINE
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#endif

struct dcache {
    static constexpr uint64_t num_sets = 16;
    static constexpr uint64_t num_ways = 4;
    static constexpr uint64_t num_lines = num_sets * num_ways;
    static constexpr uint64_t evicts_per_line = 2;
    static constexpr uint64_t line_size = 64;
    static constexpr uint64_t full_size = line_size * num_lines;

    struct alignas(line_size) line_t { // 512-bit cache line
        char data[line_size];
    };

    static ALWAYS_INLINE void evict() {
        evict_sw(0, to_L2, 0, 0, num_sets - 1);
        evict_sw(0, to_L2, 1, 0, num_sets - 1);
        evict_sw(0, to_L2, 2, 0, num_sets - 1);
        evict_sw(0, to_L2, 3, 0, num_sets - 1);

        WAIT_CACHEOPS;
        FENCE;
    }

    line_t lines[num_lines];
};
static_assert(sizeof(dcache) == dcache::full_size, "The compiler shouldn't be padding");

// Unroll a store N times with increasing offset
template <uint8_t N>
ALWAYS_INLINE void batch_store(uint64_t base_addr) {
    static_assert(N < 32, "Invalid size for batch_store");
    batch_store<N - 1>(base_addr);
    static constexpr uint16_t offset = (N - 1) * 64;
    asm("sd zero, %[offset](%[base_addr])\n" ::[base_addr] "r"(base_addr), [offset] "i"(offset) :);
}

template <>
ALWAYS_INLINE void batch_store<0>(uint64_t base_addr) {
    (void)base_addr;
}

// Unroll a load N times with increasing offset
template <uint8_t N>
ALWAYS_INLINE void batch_load(uint64_t base_addr) {
    static_assert(N < 32, "Invalid size for batch_load");
    batch_load<N - 1>(base_addr);
    static constexpr uint16_t offset = (N - 1) * 64;
    asm("ld zero, %[offset](%[base_addr])\n" ::[base_addr] "r"(base_addr), [offset] "i"(offset) :);
}

template <>
ALWAYS_INLINE void batch_load<0>(uint64_t base_addr) {
    (void)base_addr;
}
