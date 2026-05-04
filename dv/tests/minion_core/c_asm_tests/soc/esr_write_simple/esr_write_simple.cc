/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"
#include "stdio.h"
#include "et_test_common.h"
#include "cacheops.h"

#define NEIGH0_IPI_REDIRECT_PC 0x80100040
#define NEIGH0_MINION_BOOT_ADDR 0x80D00018

int main()
{
// Init marker
   C_TEST_START;

    volatile uint64_t *esr_poll_value;
    volatile uint64_t *esr_write;

    // Wait for the debugger to write this ESR
    esr_poll_value = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
    while ( *esr_poll_value != 0xFE );

    // Sending request to ESRs
    esr_write = (volatile uint64_t*) NEIGH0_MINION_BOOT_ADDR;
    *esr_write = 0xEE;

   // Finish minion test (it will perform a WFI)
    C_TEST_PASS;
    return 0;
}
