/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

#include "kernel.h"
#define LOOP_SIZE 1024*1024*2

int main() {

   sc_pv_clear_l2_scratchpad(0xff);

   uint64_t res = shire_cache_power_virus(LOOP_SIZE);
   if (res == 0) {
      C_TEST_PASS;
   }
   else {
      C_TEST_FAIL;
   }
   return 0;
}
