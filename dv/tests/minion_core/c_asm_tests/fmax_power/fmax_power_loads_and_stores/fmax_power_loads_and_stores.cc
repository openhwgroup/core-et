/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"
#include "fmax_power_loads_and_stores.h"


int main()
{

   uint64_t loop_count = 1;
   //uint64_t loop_count = 1000000;
   uint64_t pass;


   // Init marker
   C_TEST_START;

   pass = fmax_power_loads_and_stores(loop_count);


   if (pass != 0) {
      C_TEST_FAIL;
   } else {
      C_TEST_PASS;
   }

   return 0;
}

