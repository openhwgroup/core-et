/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "et_test_common.h"
#include "minion.h"
#include "perf_cntr_diag.h"
#include "dcache_diag.h"

volatile dcache main_mem[2]; // Working memory

// Runs a simple test kernel with any counter
template <typename Counter, uint8_t NBatch = 1>
ALWAYS_INLINE void runtest() {
    static_assert(dcache::num_lines % NBatch == 0, "Invalid value for NBatch");

    constexpr PmuMinEvent event = pmu_event_L2_EVICT_REQ;
    constexpr uint64_t batch_count = NBatch;
    constexpr uint64_t batch_size = batch_count * sizeof(dcache::line_t);

    Counter::reset();

    uint64_t base_addr = (uint64_t)main_mem;
    uint64_t gold_count = 0;

    excl_mode(1);

    dcache::evict();

    Counter::configure(event);

    // Iterate over first half. No evicts here
    for (uint64_t i = 0; i < dcache::num_lines; i += batch_count) {
        batch_store<batch_count>(base_addr);
        FENCE;
        base_addr += batch_size;
        const uint64_t event_count = Counter::read();
        cmp_data(event_count, gold_count);
    }

    // // Iterate over second half. Expect NBatch evicts per iteration
    for (uint64_t i = 0; i < dcache::num_lines; i += batch_count) {
        batch_store<batch_count>(base_addr);
        FENCE;
        base_addr += batch_size;
        gold_count += batch_count * dcache::evicts_per_line;
        const uint64_t event_count = Counter::read();
        cmp_data(event_count, gold_count);
    }

    // Evict dcache again. This time count
    for (uint64_t way = 0; way < dcache::num_ways; ++way) {
        evict_sw(0, to_L2, way, 0, dcache::num_sets - 1);
        WAIT_CACHEOPS;
        FENCE;
        gold_count += dcache::num_sets * dcache::evicts_per_line;
        const uint64_t event_count = Counter::read_twice();
        cmp_data(event_count, gold_count);
    }

    excl_mode(0);
}

int main() {
    C_TEST_START

    // Repeat the same test with all counters
    runtest<PmuCounter<3>, 4>();
    runtest<PmuCounter<4>, 4>();
    runtest<PmuCounter<5>, 4>();
    runtest<PmuCounter<6>, 4>();

    C_TEST_PASS
    return 0;
}
