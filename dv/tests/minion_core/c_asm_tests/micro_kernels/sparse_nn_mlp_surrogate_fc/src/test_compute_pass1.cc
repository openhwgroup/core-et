/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#include "test_common.h"
#include "test_compute_pass1.h"

void test_compute_pass1(uint32_t shire_id, uint32_t minion_id)
{
    __asm__ __volatile__ (
        "test_compute_pass1_start_point:\n"
        ".global test_compute_pass1_start_point\n"
    );

#ifndef SINGLE_PASS
    #include "test_compute_pass.h"
#endif
    __asm__ __volatile__ (
        "test_compute_pass1_end_point:\n"
        ".global test_compute_pass1_end_point\n"
    );
}

