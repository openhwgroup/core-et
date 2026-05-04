/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

#include "test_mprot.h"
#include "kernel.h"

#define LOOP_SIZE 1

int main() {

   set_dram_32gb();

   uint64_t res = sc_noc_power_virus(LOOP_SIZE);
   if (res == 0) {
      C_TEST_PASS;
   }
   else {
      C_TEST_FAIL;
   }
   return 0;
}
