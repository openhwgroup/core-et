/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "et_test_common.h"
#include "minion.h"
#include "perf_cntr_diag.h"
#include "fcc.h"
#include "dcache_diag.h"

volatile dcache main_mem[1]; // Working memory

// Runs a simple test kernel with any counter
template <typename Counter, uint8_t NBatch = 1>
ALWAYS_INLINE void runtest() {
    static_assert(dcache::num_lines % NBatch == 0, "Invalid value for NBatch");
    constexpr uint64_t batch_count = NBatch;
    constexpr uint64_t batch_size = batch_count * dcache::line_size;

    const auto tid = get_thread_id();
    const PmuMinEvent event = tid ? pmu_event_DCACHE_MISSES1 : pmu_event_DCACHE_MISSES0;

    Counter::reset();

    excl_mode(1);

    dcache::evict();

    Counter::configure(event); // Start counting..

    uint64_t base_addr = (uint64_t)main_mem;
    uint64_t gold_count = 0;

    // Load entire memory: Count batch per iteration
    for (uint64_t i = 0; i < dcache::num_lines; i += batch_count) {
        batch_load<batch_count>(base_addr);
        FENCE;
        base_addr += batch_size;
        gold_count += batch_count;
        const uint64_t event_count = Counter::read();
        cmp_data(event_count, gold_count);
    }

    // Reset address
    base_addr = (uint64_t)main_mem;

    // Load again: This time should hit
    for (uint64_t i = 0; i < dcache::num_lines; i += batch_count) {
        batch_load<batch_count>(base_addr);
        FENCE;
        base_addr += batch_size;
        const uint64_t event_count = Counter::read();
        cmp_data(event_count, gold_count);
    }

    excl_mode(0);
}

int main() {
    C_TEST_START;

    // Baseline test
    runtest<PmuCounter<3>, 1>();
    runtest<PmuCounter<4>, 1>();
    runtest<PmuCounter<5>, 1>();
    runtest<PmuCounter<6>, 1>();

    // This will mismatch, see DOC-136
    // runtest<PmuCounter<3>, 4>();
    // runtest<PmuCounter<4>, 4>();
    // runtest<PmuCounter<5>, 4>();
    // runtest<PmuCounter<6>, 4>();

    C_TEST_PASS;
    return 0;
}
