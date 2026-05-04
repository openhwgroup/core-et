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

#define LOOP_SIZE 128
#define PING_PONG_SIZE 8192
#define DELAY PING_PONG_SIZE/LOOP_SIZE

uint64_t run_kernels(uint64_t hart_id) {

    uint64_t kernel_status = PASS;

    if ((hart_id&0x1) == 0) {
       for (int i = 0; i < PING_PONG_SIZE; i++) {
          kernel_status |= sc_power_virus::shire_cache_power_virus_thread0(LOOP_SIZE);
          if (kernel_status != PASS) return kernel_status;
          uint64_t delay = i*DELAY;
          slt_soc_fcc_barrier_with_delay(delay);
       }
    }
    else {
       for (int i = 0; i < PING_PONG_SIZE; i++) {
          kernel_status |= vpu_txfma::vpu_power_virus(32*LOOP_SIZE) & 0x1;
          if (kernel_status != PASS) return kernel_status;
          uint64_t delay = i*DELAY;
          slt_soc_fcc_barrier_with_delay(delay);
       }
    }
    slt_barrier(2);

    return kernel_status;
}

int main() {
    C_TEST_START;
    uint64_t hart_id = getHartId();

    // Clear L2 SCP
    init_scratchpads(hart_id);

    if (hart_id == 0) {
        my_printf ("Hart %d - Starting Minion Shire Ping Pong Power Virus", hart_id);
    }
    // SP will synchronize all the threads
    wait_for_sp();

    // Run all the Minion Shire tests
    uint64_t kernel_status = run_kernels(hart_id);
    
    min_done_notify_sp(kernel_status, hart_id, 0);
    if (kernel_status != PASS) {
        C_TEST_FAIL;
    }

    C_TEST_PASS;
}
