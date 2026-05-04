/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

 * */

#ifndef _FC_UTILS_
#define _FC_UTILS_

#include "hal_device.h"


inline void __attribute__((always_inline)) fc_send_mbox_sync_to_sp( void)
{
    *((uint64_t*)R_PU_MBOX_MM_SP_BASEADDR) = 0xbabebeef;
    return;
}

#endif
