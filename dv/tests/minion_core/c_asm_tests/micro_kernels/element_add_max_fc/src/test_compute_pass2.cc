/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#include "test_common.h"
#include "test_compute_pass2.h"

void test_compute_pass2(uint32_t shire_id, uint32_t minion_id)
{
    __asm__ __volatile__ (
        "test_compute_pass2_start_point:\n"
        ".global test_compute_pass2_start_point\n"
    );

    #include "test_compute_pass.h"

    __asm__ __volatile__ (
        "test_compute_pass2_end_point:\n"
        ".global test_compute_pass2_end_point\n"
    );
}

