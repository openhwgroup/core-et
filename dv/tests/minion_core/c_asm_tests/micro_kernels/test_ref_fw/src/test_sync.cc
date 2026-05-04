/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "test_sync.h"

void test_sync(uint32_t shire_id, uint32_t minion_id)
{
    __asm__ __volatile__ ("csrw   0x822, x0"); // Go to sleep
}

