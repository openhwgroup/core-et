/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

#include "kernel.h"
#include "barrier.h"
#define LOOP_SIZE 1024*1024*16

int main() {
   sc_pv_clear_l2_scratchpad(0xff);
   soc_barrier(34);
   uint64_t res = noc_power_virus(LOOP_SIZE);
   if (res == 0) {
      C_TEST_PASS;
   }
   else {
      C_TEST_FAIL;
   }
   return 0;
}
