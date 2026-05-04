/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "cacheops.h"


int main()
{
   volatile char __attribute__((aligned(4096))) ptr[4096];

   // Init marker
   C_TEST_START;

   // Only thread 0 executes this
   if (get_hart_id() == 0) {

      //      use_tmask  addr                     num_lines  stride  id
      //      ---------  -----------------------  ---------  ------  --
      lock_va(false,     (uint64_t)(void*)(ptr),  15,        64,     0 );

      WAIT_CACHEOPS;

      for (int i = 0; i < 1024; i += 16) {
         ptr[i] = i;
      }

   }

   // End marker
   C_TEST_PASS;

   return 0;
}
