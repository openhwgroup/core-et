/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "common.h"
#include "esr.h"
#include "print_dbg.h"
#include "mbox_intr.h"

#include "kernel.h"
#include "slt_barrier.h"
#include "bringup_helper.h"

#define PASS 0x0
#define LOOP_SIZE 1

uint64_t run_kernels(uint64_t hart_id) {

    uint64_t thread_id = hart_id & 0x1;
    uint64_t kernel_status = PASS;

    /* Intpipe ALU */
    // Implicitly covered by the test execution



    /* Dcache S1 and S3 stages */
    kernel_status |= dcache_loads_stores::fmax_power_loads_and_stores(3) & 0x1;
    if (kernel_status != PASS) return kernel_status;
    slt_barrier(2);



    /* VPU */
    kernel_status |= (vpu_txfma::vpu_power_virus(3) & 0x1) << 1;
    if (kernel_status != PASS) return kernel_status;
    slt_barrier(2);

    if (thread_id == 0) {
        kernel_status |= (vpu_tima8a32::vpu_tima_power_virus(3) & 0x1) << 2;
    }
    if (kernel_status != PASS) return kernel_status;
    slt_barrier(2);

    if (thread_id == 0) {
        kernel_status |= (vpu_bypass::vpu_bypass_power_virus(3) & 0x1) << 3;
    }
    if (kernel_status != PASS) return kernel_status;
    slt_barrier(2);



    /* ET-Link interfaces */
    // Implicitly covered by the test execution on multiple minions simultaneously



    /* Minion Core to Neighborhood */
    // Implicitly covered by the test execution on multiple minions simultaneously



    /* Neighborhood to Shire Channel/Cache */
    // Implicitly covered by the test execution on multiple minions simultaneously



    return kernel_status;
}

int main() {
    C_TEST_START;
    uint64_t hart_id = getHartId();

    // Clear L2 SCP
    init_scratchpads(hart_id);

    for (int loop = 0; loop < LOOP_SIZE; loop++) {
        if (hart_id == 0) {
            my_printf ("Hart %d - Test Start %x", hart_id, loop);
        }
        // SP will synchronize all the threads
        wait_for_sp();

        // Run all the Minion Shire tests
        uint64_t kernel_status = run_kernels(hart_id);
        
        min_done_notify_sp(kernel_status, hart_id, loop);
        if (kernel_status != PASS) {
            C_TEST_FAIL;
        }
    }
    C_TEST_PASS;
}
