/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does a series stores and loads in a walking 1s pattern
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "common.h"
#include "esr.h"
#include "print_dbg.h"
#include "mbox_intr.h"
#include "bringup_helper.h"

int main() {

    uint64_t kernel_status = 0x0;
    C_TEST_START;

    uint64_t hart_id = getHartId();
    init_scratchpads(hart_id);
    synchronize_dv_harts(hart_id);
    kernel_status = (uint64_t)kernel_blast_mem(hart_id, 100) ;

#ifdef SP_ENDS_TEST
    min_done_notify_sp(kernel_status, hart_id, 0x0);
#endif

    if (kernel_status) {
	C_TEST_FAIL;
    }

    C_TEST_PASS;
}
