/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include <stdio.h>
#include <stdlib.h>
#include "et_test_common.h"
#include "minion.h"
#include "perf_cntr_diag.h"
#include "utils.h"
#include "tensors.h"

#ifdef MINSHIRE_GATESIM_ENABLE
#define TIMEOUT 500
#endif

#define NUM_TENSOR_INST 12

#define BLOB_SIZE 1048576 // 1M
#define FP_BLOB_SIZE (BLOB_SIZE/2)
// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start;
__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:        \n\t"
      "      .align     12         \n\t"
      "      .incbin    \"data.raw\"  \n\t"
);

// Runs a simple test kernel with any counter
template <typename Counter>
ALWAYS_INLINE void runtest() {
    const int mid = get_minion_id();
    const int tid = get_thread_id();
    const PmuMinEvent event = pmu_event_TREDUCE_INST;
    const uint64_t gold_tensor_inst = (tid == 0) ? NUM_TENSOR_INST : 0;

    // Reset counter
    Counter::configure(pmu_event_NOCOUNT);
    Counter::write(0);

    // Test setup would go here
    if (tid == 0) {
        volatile cl_data_t *buffer = &_binary_blob_bin_start;
        uint64_t idx = (mid*mid) % (FP_BLOB_SIZE/sizeof(cl_data_t));
        uint64_t ptr = (uint64_t)(void*)buffer[idx].data[0];
        pmu_init_fp_regs(ptr);
        FENCE;
    }
    else {
        setup_tensor_th1_mtrap_vector();
    }

    Counter::configure(event); // Start counting..

    // Test kernel
    for (uint64_t i = 0; i < gold_tensor_inst; i++) {
        uint64_t start_reg  = 0;
        uint64_t num_reg    = 1;
        uint64_t tree_depth = 0;
        uint64_t operation  = 0;
        tensor_reduce_auto(start_reg, operation, num_reg, tree_depth);
    }

    WAIT_TENSOR_REDUCE;

    // Read counter and check
    //const uint64_t event_count = Counter::read();
    //cmp_data(event_count, gold_tensor_inst);
}

int main() {
    C_TEST_START

    const int tid = get_thread_id();
    if (tid == 1) {
       wait_for_everyone();
    }

    // Repeat the same test with all counters
    runtest<PmuCounter<3> >();
    runtest<PmuCounter<4> >();
    runtest<PmuCounter<5> >();
    runtest<PmuCounter<6> >();
    //runtest<PmuCounter<7> >();
    //runtest<PmuCounter<8> >();

    if (tid == 0) {
       wait_for_everyone();
    }

    C_TEST_PASS
    return 0;
}


