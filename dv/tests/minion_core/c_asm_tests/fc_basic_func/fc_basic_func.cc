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
#define BASE_ADDR 0x0000008100000000

int main()
{

  C_TEST_START; 

  volatile uint64_t hart_id = get_hart_id();
  volatile uint64_t thread_id  = (hart_id & 1);
  volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
  volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);

  volatile uint64_t* ptr = (uint64_t*)BASE_ADDR;
  volatile uint64_t timeout = TIMEOUT;

  if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {
    while (*ptr != 2) {
      evict_va(0, to_Mem, BASE_ADDR);
      WAIT_CACHEOPS
      timeout--;
      if (timeout == 0) C_TEST_FAIL
    }
  } else if (hart_id == 0) {
    *ptr=0;
    *ptr+=2;
    evict_va(0, to_Mem, BASE_ADDR);
    WAIT_CACHEOPS  
  }

  C_TEST_PASS;
}