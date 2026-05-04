/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "et_barrier.h"

#define LOOP_SIZE 128

#include "../../shire_cache/power/kernel.h"
#include "../../power_vpu/kernel.h"

#define PASS 0
#define PING_PONG_SIZE 8192
#define DELAY PING_PONG_SIZE/LOOP_SIZE

int main() {
   sc_pv_clear_l2_scratchpad(0xff);

   uint64_t hid = get_hart_id();

   uint64_t res = PASS;
   if ((hid&0x1) == 0) {
      for (int i = 0; i < PING_PONG_SIZE; i++) {
         res = shire_cache_power_virus_thread0(LOOP_SIZE);
         if (res != PASS) break;
         uint64_t delay = i*DELAY;
         soc_fcc_barrier_with_delay(34, delay);
      }
   }
   else {
      for (int i = 0; i < PING_PONG_SIZE; i++) {
         res = vpu_power_virus(32*LOOP_SIZE);
         if (res != PASS) break;
         uint64_t delay = i*DELAY;
         soc_fcc_barrier_with_delay(34, delay);
      }
   }

   if (res == PASS) {
      C_TEST_PASS;
   }
   else {
      C_TEST_FAIL;
   }
   return 0;
}
