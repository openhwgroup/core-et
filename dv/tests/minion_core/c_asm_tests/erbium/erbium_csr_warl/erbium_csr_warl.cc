/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "et_test_common.h"
#include "minion.h"

#define WRITE_READ_CSR(OUT, NAME, VAL)          \
__asm__ __volatile__(                           \
    "csrs " #NAME ", %[val]\n"                  \
    "csrr %[out], " #NAME " \n"                 \
    : [out] "=r" (OUT)                          \
    : [val] "r" (VAL)                           \
    :                                           \
    );

// TEST_CSR macro test a CSR for RO/WARL fields
// - Name: name of the CSR
// - EXP_VALUE: Expected valeu to be read from the CSR
// - MASK: Mask with the bits that want to be checked
// macro will write all the masked bits, read back the CSR and check if the masked read value matches the expected value
#define TEST_CSR(NAME, EXP_VALUE, MASK) in = -1UL & MASK; WRITE_READ_CSR(out, NAME, in); if ((out & MASK) != EXP_VALUE) C_TEST_FAIL;
#define TEST_CSR_T0(NAME, EXP_VALUE, MASK) if (get_thread_id() == 0) {TEST_CSR(NAME, EXP_VALUE, MASK)}


int main()
{
    setup_mtrap_vector();

    uint64_t thid = get_thread_id();
    uint64_t out = 0;
    uint64_t in = -1;
    TEST_CSR(matp, 0ULL, -1ULL)
    TEST_CSR(satp, 0ULL, -1ULL)
    TEST_CSR(tdata3, 0ULL, -1ULL)
    TEST_CSR(mstatus, 0ULL, 1ULL << 18) // mstatus.sum RO(0)
    TEST_CSR(sstatus, 0ULL, 1ULL << 18) // sstatus.sum RO(0)

    // Test CSRs that can't read back it's value
    // The test will just generate the traffic but an assertion/scoreboard
    // must check the internal value
    // ---------------------------------------
    // Tensor coop
    // - Writing tensor_coop[19:16] from 0 to 0xF (coopneighmask)
    if (thid == 0) {
        for (uint64_t neighid = 0; neighid < 16; ++neighid)
            asm volatile ("csrw tensor_coop, %[id]\n" :: [id] "r" (neighid << 16):);
    }
    
    C_TEST_PASS
}
