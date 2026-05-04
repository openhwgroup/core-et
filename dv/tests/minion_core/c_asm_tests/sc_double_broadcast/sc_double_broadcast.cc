/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"

// chip-level broadcast, and neigh ESR broadcast
#define MASTER_SHIRE 32
#define MASTER_MINION 0
#define MASTER_THREAD 0

int main()
{

//  volatile uint64_t hart_id = get_hart_id();
  volatile uint64_t thread_id  = get_thread_id();
  volatile uint64_t minion_id = get_minion_id();
  volatile uint64_t shire_id  = get_shire_id();

  C_TEST_START;
  if ((shire_id == MASTER_SHIRE) && (minion_id == MASTER_MINION) && (thread_id == MASTER_THREAD)) {
    broadcast_req (PP_MACHINE,       REGION_OTHER, 0x3C0C,           0xffffffff, 0x12345678);
  }
  C_TEST_PASS;
}