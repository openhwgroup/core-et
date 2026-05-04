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
    constexpr PmuMinEvent event = pmu_event_L2_MISS_REQ;
    constexpr uint64_t batch_count = NBatch;
    constexpr uint64_t batch_size = batch_count * dcache::line_size;

    Counter::reset();

    // Since both threads share the L1 dcache
    // we need to execute in exclusive mode
    // in order to control interference
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

    // Reset base address
    base_addr = (uint64_t)main_mem;

    // Load again: This time the counter shouldn't change
    for (uint64_t i = 0; i < dcache::num_lines; i += batch_count) {
        batch_load<batch_count>(base_addr);
        FENCE;
        base_addr += batch_size;
        const uint64_t event_count = Counter::read();
        cmp_data(event_count, gold_count);
    }

    excl_mode(0);
}

template <typename Counter, uint8_t NBatch = 1>
ALWAYS_INLINE void runtest_coop() {
    static_assert(dcache::num_lines % NBatch == 0, "Invalid value for NBatch");
    constexpr PmuMinEvent event = pmu_event_L2_MISS_REQ;
    constexpr uint64_t batch_count = NBatch;
    constexpr uint64_t batch_size = batch_count * dcache::line_size;

    constexpr uint64_t gold_count = dcache::num_lines;

    Counter::reset();

    constexpr uint64_t t1_offset = dcache::full_size / 2;
    const uint64_t minion_id = get_minion_id();
    const uint64_t thread_id = get_thread_id();

    uint64_t base_addr = (uint64_t)main_mem + (thread_id ? t1_offset : 0);

    if (thread_id == 0) {
        wait_for_credit(0);                 // Wait for T1
        excl_mode(1);                       // Evict dcache
        dcache::evict();                    // ..
        excl_mode(0);                       // ..
        Counter::configure(event);          // Start counting
        give_credit_local(minion_id, 1, 0); // Signal T1
        wait_for_credit(0);                 // Finish handshake
    } else {
        give_credit_local(minion_id, 0, 0); // Signal T0
        wait_for_credit(0);                 // Wait for T0.. at this point the dcache is "empty"
        Counter::configure(event);          // Start counting
        give_credit_local(minion_id, 0, 0); // Finish handshake
    }

    // Test kernel: T0/T1 iterate over first/second half of memory respectively
    for (uint64_t i = 0; i < dcache::num_lines / 2; i += batch_count) {
        batch_load<batch_count>(base_addr);
        base_addr += batch_size;
    }

    FENCE;

    if (thread_id == 0) {
        wait_for_credit(0);                           // Wait for T1
        const uint64_t event_count = Counter::read(); // Check counter
        cmp_data(event_count, gold_count);            // ..
        give_credit_local(minion_id, 1, 0);           // Signal T1
        wait_for_credit(0);                           // Finish handshake
    } else {
        give_credit_local(minion_id, 0, 0); // Signal T0
        wait_for_credit(0);                 // Wait for T0.. at this point the dcache is "full"
        const uint64_t event_count = Counter::read(); // Check counter
        cmp_data(event_count, gold_count);            // ..
        give_credit_local(minion_id, 0, 0);           // Finish handshake
    }
}

int main() {
    C_TEST_START;

    // Baseline test
    runtest<PmuCounter<3>, 1>();
    runtest<PmuCounter<4>, 1>();
    runtest<PmuCounter<5>, 1>();
    runtest<PmuCounter<6>, 1>();

    // Fill up replay queue
    runtest<PmuCounter<3>, 16>();
    runtest<PmuCounter<4>, 16>();
    runtest<PmuCounter<5>, 16>();
    runtest<PmuCounter<6>, 16>();

    // Run threads in parallel
    runtest_coop<PmuCounter<3>, 8>();
    runtest_coop<PmuCounter<4>, 8>();
    runtest_coop<PmuCounter<5>, 8>();
    runtest_coop<PmuCounter<6>, 8>();

    C_TEST_PASS;
    return 0;
}
