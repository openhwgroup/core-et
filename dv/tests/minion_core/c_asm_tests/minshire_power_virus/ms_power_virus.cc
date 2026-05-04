/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

#define LOOP_SIZE 1024*1024*2

#include "../shire_cache/power/kernel.h"
#include "../power_vpu/kernel.h"

int main() {
   sc_pv_clear_l2_scratchpad(0xff);

   uint64_t res;
   uint64_t hid = get_hart_id();
   if ((hid&0x1) == 0) {
      res = shire_cache_power_virus_thread0(LOOP_SIZE);
   }
   else {
      res = vpu_power_virus(32*LOOP_SIZE);
   }

   if (res == 0) {
      C_TEST_PASS;
   }
   else {
      C_TEST_FAIL;
   }
   return 0;
}
