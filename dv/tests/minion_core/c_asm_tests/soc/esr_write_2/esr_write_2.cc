/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"
#include "stdio.h"

#define NEIGH0_IPI_REDIRECT_PC 0x80100040
#define NUM_ITER 10

volatile uint64_t *esr_poll_value __attribute__((section(".address_array"))) = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC;


int main()
{
// Init marker
   C_TEST_START;

    // Wait for the debugger to write this ESR
    while ( *esr_poll_value != 0xFE );

    // Start sending requests to ESRs
    for ( int i = 0; i < NUM_ITER; i++) {

        // Perform write request
        *esr_poll_value = 0xFE;
    }

    // Finish minion test (it will perform a WFI)
   C_TEST_PASS;
   return 0;
}



