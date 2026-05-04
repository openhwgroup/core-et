/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

#include "kernel.h"

#define LOOP_SIZE 256*1024*1024

int main() {

   uint64_t res = 0;
   int tid = get_hart_id() & 0x1;
   if (tid == 0) {
      res = vpu_bypass_power_virus(LOOP_SIZE);
   }

   if (res == 0) {
      C_TEST_PASS;
   }
   else {
      C_TEST_FAIL;
   }

   return 0;
}
