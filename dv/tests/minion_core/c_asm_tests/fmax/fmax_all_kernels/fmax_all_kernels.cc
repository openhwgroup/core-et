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

int main()
{
    C_TEST_START;
    uint64_t kernel_status = 0x0;
    uint16_t loop = 0x0;
    uint64_t hart_id = getHartId();

    init_scratchpads(hart_id);

#ifdef FMAX_LOOP_TEST
    while (loop < 0xffff)
#endif
    {
        if (hart_id == 0) {
            my_printf ("Hart %d - Test Start %x", hart_id, loop);
        }
        // SP will synchronize all the threads
        wait_for_sp();
        synchronize_dv_harts(hart_id);
        {
            // Other kernels go here
            
            kernel_status |= kernel_blast_mem(hart_id, 10) ;
            //kernel_status |= fmax_power_loads_and_stores(10);
/*
            // VPU
            kernel_status |= vpu_power_virus(10);
            if ((hart_id&0x1) == 0) {
               kernel_status |= vpu_tima_power_virus(10);
               kernel_status |= vpu_bypass_power_virus(10);
            }

            // SCW
            // PRE: The test assumes that L2 SCP of all the Shires (0-33) are initialized/cleared
            kernel_status |= shire_cache_power_virus(10);
            
            // NoC
            // PRE: The test assumes that L2 SCP of all the Shires (0-33) are initialized/cleared
            kernel_status |= noc_power_virus(10);
            kernel_status |= fmax_noc_amo(10);
            kernel_status |= fmax_noc_esr(10);

            // Minion Shire Power Virus (Thread 0: Shire Cache Power Virus, Thread 1: VPU Power Virus)
            if ((hart_id&0x1) == 0) {
               kernel_status |= shire_cache_power_virus_thread0(10);
            }
            else {
               kernel_status |= vpu_power_virus(10);
            }

            // Shire Cache NoC Power Virus
            kernel_status |= sc_noc_power_virus(10);
*/
        }
        min_done_notify_sp(kernel_status, hart_id, loop++);
        if (kernel_status) {
            C_TEST_FAIL;
        }
    }
    C_TEST_PASS;
}
