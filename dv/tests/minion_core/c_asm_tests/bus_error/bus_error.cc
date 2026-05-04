/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test exercises basic Bus Error functionality. Should be
//               run with the NoC enabled.
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

int main()
{
    // Setup a basic trap handler that will continue to the next instruction.
    // We expect the trap to appear at some point after the access to *ptr and
    // before ``if (val)``
    __asm__ volatile(
    "   la t0, 0f\n"
    "   csrw mtvec, t0\n"
    "   j 1f\n"
    "   .align 12\n"
    "0: mret\n"
    "1:"
    ::: "t0" );

    uint64_t val = 0;
    uint64_t hartid = get_hart_id();
    volatile uint64_t *ptr = (volatile uint64_t*) (0x8fffff0000ull);
    C_TEST_START;
    val = *ptr + hartid + 1;
    delay(500);
    if (val) {
        C_TEST_FAIL;
    } else {
        C_TEST_PASS;
    }
    return 0;
}
