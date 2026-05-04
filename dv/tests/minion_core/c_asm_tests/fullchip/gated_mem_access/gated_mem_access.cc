/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


// Accessing L3 of gated shire

#include "cpu.h" 
#include "tb.h" 
#include "soc.h" 
#include "macros.h" 
#include "plic_api.h"
#include "fcc.h"
#include "esr.h"
#include <inttypes.h>
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"
#include "common.h"

#define master_counter 0
#define L3_data_shire0 0x0000008100000000

int main()
{
  uint64_t*   baseAddr =  (uint64_t*) R_PU_MBOX_MM_SP_BASEADDR;

  C_TEST_START

  volatile uint64_t hart_id = getHartId();
  volatile uint64_t thread_id  = (hart_id & 1);
  volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
  volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
  volatile long data = 0;
  long*    data_ptr = 0;

  if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {
    wait_for_credit(master_counter);
    store(L3_data_shire0, 5);
    evict_va(0, 2, L3_data_shire0);
    WAIT_CACHEOPS
    data_ptr = (long*) L3_data_shire0;
    data = *data_ptr;
    if (data != 5) {
      C_TEST_FAIL
    } 
  } else if (shire_id == 0) {
    if(hart_id == 0) *baseAddr = 0xbabe;
    WFI
  }
  	
  C_TEST_PASS
}