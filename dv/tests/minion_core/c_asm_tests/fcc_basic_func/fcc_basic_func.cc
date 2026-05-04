/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"
#include "minion.h"
#include "macros.h"
#include "fcc.h"
#include "esr.h"
#include <inttypes.h>

int main()
{

  C_TEST_START; 

  volatile uint64_t hart_id = get_hart_id();
  volatile uint64_t thread_id  = (hart_id & 1);
  volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
  volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);


  if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {
    wait_for_credit(0);    
    give_credit(0, 0, 0, 0);
  } else if (hart_id == 0) {
    give_credit(32, 0, 0, 0);
    wait_for_credit(0);
  }
  C_TEST_PASS;
}