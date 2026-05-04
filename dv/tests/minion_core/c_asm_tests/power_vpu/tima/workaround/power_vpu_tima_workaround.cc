/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

#include "kernel.h"

#define LOOP_SIZE 512*1024

int main() {

   uint64_t res = 0;
   uint64_t tid = get_thread_id();
   if (tid == 0) {
      res = vpu_tima_power_virus(LOOP_SIZE);
   }

   if (res == 0) {
      C_TEST_PASS;
   }
   else {
      C_TEST_FAIL;
   }

   return 0;
}
