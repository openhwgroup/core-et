/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "macros.h"
#include "et_test_common.h"

int main()
{
    C_TEST_START;
    //            (user,          esr region,   esr_addr(fcc reg) shire_mask, data
    broadcast_req (PP_USER,       REGION_OTHER, 0x8018,           0xffffffff, 0x234567890);
    // broadcast_req (PP_SUPERVISOR, REGION_OTHER, 0x8018,           0xaaaaaaaa, 0x234567890);
    // broadcast_req (PP_MACHINE,    REGION_OTHER, 0x8018,           0x11111111, 0x234567890);
    C_TEST_PASS;
}
