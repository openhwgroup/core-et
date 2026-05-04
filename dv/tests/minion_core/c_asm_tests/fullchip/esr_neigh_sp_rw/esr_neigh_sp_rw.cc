/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "hal_device.h"
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"

int main()
{
	uint64_t*   baseAddr =  (uint64_t*) R_PU_MBOX_MM_SP_BASEADDR;

    C_TEST_START;

    *baseAddr = 0xbabe;

    C_TEST_PASS;

} /*esr_rw_Test() */

/*****     < EOF >     *****/