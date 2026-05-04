/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "common.h"
#include "esr.h"
#include "print_dbg.h"
#include "mbox_intr.h"
#include "bringup_helper.h"

#include "kernel.h"
#include "slt_barrier.h"

#define PASS 0x0
#define LOOP_SIZE 1

uint64_t run_kernels(uint64_t hart_id) {

    uint64_t kernel_status = PASS;

    kernel_status |= blast_mem::kernel_blast_mem (hart_id, 3);
    if (kernel_status != PASS) return kernel_status;
    slt_barrier(2);

    kernel_status |= noc_power_virus::noc_power_virus(3);
    if (kernel_status != PASS) return kernel_status;
    slt_barrier(2);

    kernel_status |= sc_noc_power_virus::sc_noc_power_virus(3);
    if (kernel_status != PASS) return kernel_status;
    slt_barrier(2);

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
