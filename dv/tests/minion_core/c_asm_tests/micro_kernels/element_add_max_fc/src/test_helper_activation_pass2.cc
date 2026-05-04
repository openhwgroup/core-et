/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "test_helper_activation_pass2.h"

void test_helper_activation_pass2(uint32_t shire_id, uint32_t minion_id)
{
    __asm__ __volatile__ (
        "test_helper_activation_pass2_start_point:\n"
        ".global test_helper_activation_pass2_start_point\n"
    );

    #include "test_helper_activation_pass.h"

    __asm__ __volatile__ (
        "test_helper_activation_pass2_end_point:\n"
        ".global test_helper_activation_pass2_end_point\n"
    );
}

