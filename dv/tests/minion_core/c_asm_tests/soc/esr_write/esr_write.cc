/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"
#include "stdio.h"
#include "et_test_common.h"
#include "cacheops.h"
#include "common.h"

int main()
{
// Init marker
   C_TEST_START;

    volatile uint64_t *esr_poll_value_neigh0;
    volatile uint64_t *esr_poll_value_neigh1;
    volatile uint64_t *esr_write;

    // Wait for the debugger to write this ESR
    esr_poll_value_neigh0 = (volatile uint64_t*) ULTRASOC_TEST_ESR_AD; 
    while ( *esr_poll_value_neigh0 != 0xFE );

    // Sending request to ESRs
    esr_write = (volatile uint64_t*) ULTRASOC_TEST_ESR_1_AD;
    *esr_write = 0xEE;

    // Wait for the debugger to write this ESR
    while ( *esr_poll_value_neigh0 != 0xAA );
    *esr_write = 0xBB;

   // Finish minion test (it will perform a WFI)
    C_TEST_PASS;
    return 0;
}
