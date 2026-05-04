/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include <stdio.h>
#include <stdlib.h>
#include "et_test_common.h"
#include "minion.h"
#include "perf_cntr_diag.h"
#include "utils.h"

#ifdef MINSHIRE_GATESIM_ENABLE
#define TIMEOUT 500
#endif

// Runs a simple test kernel with any counter
template <typename Counter>
ALWAYS_INLINE void runtest() {
    const int tid = get_thread_id();
    const PmuMinEvent event = tid ? pmu_event_NOCOUNT : pmu_event_MCYCLES;
    //const uint64_t gold_count0 = 3;
    //const uint64_t gold_count1 = 5;

    // Reset counter
    Counter::configure(pmu_event_NOCOUNT);
    Counter::write(0);

    // Test setup would go here

    Counter::configure(event); // Start counting..

    // Test kernel
    __asm__ __volatile__("addi t0, x0, 0xca\n"
                         "slli t0, t0, 0x08\n"
                         "addi t0, t0, 0xfe\n" ::
                             : "t0");

    //Counter::configure(pmu_event_NOCOUNT);

    // Test kernel
    __asm__ __volatile__("addi t0, x0, 0xca\n"
                         "slli t0, t0, 0x08\n"
                         "addi t0, t0, 0xfe\n" ::
                             : "t0");

    // Read counter and check
    Counter::read();
    Counter::read();
    //const uint64_t event_count = Counter::read();
    //cmp_data(event_count, gold_count0);

    Counter::configure(pmu_event_NOCOUNT);
    Counter::write(0);

    Counter::configure(event);

    // Test kernel
    __asm__ __volatile__("nop\n"
                         "csrr t0, mhartid\n"
                         "fence\n"
                         "nop\n"
                         "slli t0, t0, 0x08\n" ::
                             : "t0");

    Counter::configure(pmu_event_NOCOUNT);

    // Test kernel
    __asm__ __volatile__("addi t0, x0, 0xca\n"
                         "slli t0, t0, 0x08\n"
                         "addi t0, t0, 0xfe\n" ::
                             : "t0");

    // Read counter and check
    Counter::read();
    //cmp_data(event_count1, gold_count1);

    //Counter::configure(pmu_event_NOCOUNT);
    Counter::write(0);

}

int main() {
    C_TEST_START

    // Repeat the same test with all counters
    runtest<PmuCounter<3> >();
    runtest<PmuCounter<4> >();
    runtest<PmuCounter<5> >();
    runtest<PmuCounter<6> >();
    //runtest<PmuCounter<7> >();
    //runtest<PmuCounter<8> >();

    C_TEST_PASS
    return 0;
}


