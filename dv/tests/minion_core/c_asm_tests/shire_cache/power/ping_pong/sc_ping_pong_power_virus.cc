/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "et_barrier.h"

#include "kernel.h"

#define LOOP_SIZE 256
#define DELAY 1024*512

#define PASS 0

int main() {

   sc_pv_clear_l2_scratchpad(0xff);

   uint64_t res;
   for (int i = 0; i < 4096; i++) {
      res = shire_cache_power_virus(LOOP_SIZE);
      if (res != PASS) break;
      soc_fcc_barrier_with_delay(34, DELAY);
   }

   if (res == PASS) {
      C_TEST_PASS;
   }
   else {
      C_TEST_FAIL;
   }
   return 0;
}
