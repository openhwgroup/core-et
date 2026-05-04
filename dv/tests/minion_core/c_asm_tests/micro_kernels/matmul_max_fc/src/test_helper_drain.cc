/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#include "test_helper_drain.h"
#include "test_helper_drain_pass0.h"
#include "test_helper_drain_pass1.h"
#include "test_helper_drain_pass2.h"

void test_helper_drain(uint32_t shire_id, uint32_t minion_id)
{
    __asm__ __volatile__ (
        "test_helper_drain_start_point:\n"
        ".global test_helper_drain_start_point\n"
    );

    // Waits until l2 scp is done
    fcc(FCC_1);

    // 3 passes
    test_helper_drain_pass0(shire_id, minion_id);
#ifndef SINGLE_PASS
    test_helper_drain_pass1(shire_id, minion_id);
    test_helper_drain_pass2(shire_id, minion_id);
#endif

    __asm__ __volatile__ (
        "test_helper_drain_end_point:\n"
        ".global test_helper_drain_end_point\n"
    );
}

