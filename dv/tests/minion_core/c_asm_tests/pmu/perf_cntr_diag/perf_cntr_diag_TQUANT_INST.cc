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
#define CL_BLOB_SIZE (BLOB_SIZE/sizeof(cl_data_t))
// Tensor Load accesses multiple addresses. To avoid acessing beyond the BLOB
// reduce the size of the BLOB to the half
#define TL_BLOB_SIZE (CL_BLOB_SIZE/2)
// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start;
__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:        \n\t"
      "      .align     12         \n\t"
      "      .incbin    \"data.raw\"  \n\t"
);

ALWAYS_INLINE void setup_tensor_csr_value(et_tensor_load_conf_t* load_conf_matrix){
    load_conf_matrix->use_tmask      = false; // xs[   63]: Not used currently
    load_conf_matrix->use_coop       = 0;     // xs[   62]: 1: cooperation, 0: no cooperation
    load_conf_matrix->transformation = 0x0UL; // xs[61:59]: Tensor Load Op dependant
    load_conf_matrix->num_lines      = 15;    // xs[ 3: 0]: num_cache_lines - 1 (Max Lines = 16)
    load_conf_matrix->dst_start      = 0;     // xs[58:53]: TO: SCP Cache line (0..47)
    load_conf_matrix->offset         = 0;     // xs[ 5: 4]
    load_conf_matrix->use_tenb       = 0;     // xs[   52]: Tensor Load Op dependant
    load_conf_matrix->stride         = 64;    // x31[47:6]: Stride in Bytes (typically a full cache line, 64 bytes)
    load_conf_matrix->id             = 0;     // x31[0]: Tensor Operation ID
}

// Runs a simple test kernel with any counter
template <typename Counter>
ALWAYS_INLINE void runtest() {
    const int mid = get_minion_id();
    const int tid = get_thread_id();
    const PmuMinEvent event = pmu_event_TQUANT_INST;
    const uint64_t gold_tensor_inst = (tid == 0) ? NUM_TENSOR_INST : 0;

    // Reset counter
    Counter::configure(pmu_event_NOCOUNT);
    Counter::write(0);

    // Test setup would go here
    if (tid == 0) {
        setup_cache_scp();
        // Load the L1SCP with non-zero data
        volatile cl_data_t *buffer = &_binary_blob_bin_start;
        et_tensor_load_conf_t  load_conf_matrix;
        setup_tensor_csr_value(&load_conf_matrix);
        uint64_t ptr = (uint64_t)(void*)buffer[(mid*mid)%TL_BLOB_SIZE].data[0];
        load_conf_matrix.addr = (uint64_t) ptr; // xs[47: 6]: FROM: Full Virtual Address
        et_tensor_load (&load_conf_matrix);
        WAIT_TENSOR_LOAD_0;
        WAIT_TENSOR_LOAD_1;
    }
    else {
        setup_tensor_th1_mtrap_vector();
    }

    Counter::configure(event); // Start counting..

    // Test kernel
    for (uint64_t i = 0; i < gold_tensor_inst; i++) {
        //            start_reg  col row scp_loc  transf9  transf8 transf7 transf6 transf5 transf4 transf3 transf2 transf1 transf0
        //           ----------  --- --- -------  -------  ------- ------- ------- ------- ------- ------- ------- ------- -------
        tensor_quant(	     0,   0,  0,      0,      10,       9,      8,      7,      6,      5,      4,      3,      2,      1);
    }

    WAIT_TENSOR_QUANT;

    // Read counter and check
    const uint64_t event_count = Counter::read();
    cmp_data(event_count, gold_tensor_inst);
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


