/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"

uint64_t fmax_power_loads_and_stores (uint64_t loop_count) {

   uint64_t pass;

   volatile uint64_t hart_id = get_hart_id(); 
   // Use hart_id + 1 as increment value. Was using hart_id
   // previously, but that meant H0 was doing nothing
   uint64_t wr_value = hart_id + 1;

   uint8_t base_addr[64*16*4*2];

   //// Init marker
   //C_TEST_START;

   init_l1d((uint32_t*)base_addr);
   FENCE;

   for (uint64_t ii = 0; ii < loop_count; ++ii) {
      incr_l1d((uint32_t*)base_addr, wr_value);
   }

   pass = check_l1d((uint32_t*)base_addr, wr_value, loop_count);

   return pass;

}

