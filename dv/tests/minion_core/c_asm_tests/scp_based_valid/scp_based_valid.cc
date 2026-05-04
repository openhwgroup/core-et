/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"
#include "minion.h"
#include "macros.h"
#include "fcc.h"
#include "esr.h"
#include <inttypes.h>


#define MASTER 32
#define TIMEOUT 500

int main()
{
  C_TEST_START; 

  volatile uint64_t hart_id = get_hart_id();
  volatile uint64_t thread_id  = (hart_id & 1);
  volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
  volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
  volatile uint64_t timeout = TIMEOUT;
  if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
    s32_scp_init();
    give_credit(0, 0, 0, 0);
  }
  if (hart_id == 0) {
    wait_for_credit(0);
  }

  if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {
    check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR);
  } else if (hart_id == 0) {
    atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR);
  }
  C_TEST_PASS;
}