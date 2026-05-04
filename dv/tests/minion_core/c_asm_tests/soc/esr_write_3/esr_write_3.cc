/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"
#include "stdio.h"
#include "et_test_common.h"
#include "cacheops.h"

#define NEIGH0_IPI_REDIRECT_PC 0x80100040
#define NEIGH0_TXT_IMG_TAB_PTR 0x80108010

int main()
{
// Init marker
   C_TEST_START;
    volatile uint64_t *esr_poll_value_neigh0;
    volatile uint64_t *esr_write;

    // Sending request to ESRs, should not be captured
    esr_write = (volatile uint64_t*) NEIGH0_TXT_IMG_TAB_PTR;
    *esr_write = 0xE0;

    // Wait for the debugger to write this ESR
    esr_poll_value_neigh0 = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
    while ( *esr_poll_value_neigh0 != 0x1234 );

    // Sending request to ESRs, should be captured
    esr_write = (volatile uint64_t*) NEIGH0_TXT_IMG_TAB_PTR;
    *esr_write = 0xEE;

   // Finish minion test (it will perform a WFI)
    C_TEST_PASS;
    return 0;
}
