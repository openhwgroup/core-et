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
    const int event = tid ? pmu_neigh_event_MIN_ICACHE_RSP : pmu_neigh_event_MIN_ICACHE_REQ;
    //const uint64_t gold_count = 3;

    // Reset counter
    Counter::configure(pmu_event_NOCOUNT);
    Counter::write(0);

    // Test setup would go here

    Counter::configure(event); // Start counting..

    // Test kernel
    //asm volatile("lui t0, x0, 0xdead\n"
    //             "slli t0, t0, 0x08\n"
    //             "addi t0, t0, 0xfe\n" ::
    //                 : "t0");

    asm volatile ("li t0, 0xdeadaaa0\n" ::
                       : "t0");

    const_instr_routine();

    // Read counter and check
    Counter::read();
    asm volatile ("li t0, 0xdeadaaa1\n" ::
                       : "t0");

    // Reset counter
    Counter::configure(pmu_event_NOCOUNT);
    Counter::write(0);

}

int main() {
    C_TEST_START

    // Repeat the same test with all counters
    //runtest<PmuCounter<3> >();
    //runtest<PmuCounter<4> >();
    //runtest<PmuCounter<5> >();
    //runtest<PmuCounter<6> >();
    runtest<PmuCounter<7> >();
    runtest<PmuCounter<8> >();

    C_TEST_PASS
    return 0;
}


