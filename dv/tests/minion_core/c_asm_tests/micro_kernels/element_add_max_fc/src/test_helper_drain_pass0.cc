/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#include "test_common.h"
#include "test_helper_drain_pass0.h"

void test_helper_drain_pass0(uint32_t shire_id, uint32_t minion_id)
{
    __asm__ __volatile__ (
        "test_helper_drain_pass0_start_point:\n"
        ".global test_helper_drain_pass0_start_point\n"
    );

    #include "test_helper_drain_pass.h"

    __asm__ __volatile__ (
        "test_helper_drain_pass0_end_point:\n"
        ".global test_helper_drain_pass0_end_point\n"
    );
}

