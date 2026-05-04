/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"

int main()
{
   volatile int __attribute__((aligned(4096))) ptr[1024];
   // Init marker
   C_TEST_START;


   // Initialize array
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i;
   }


   // Lock L1 lines
   for (int i = 0; i < 3; i++)
   {
      //      use_tmask  addr                           num_lines  stride  id
      //      ---------  ---------------------------    ---------  ------  --
      lock_va(false,     (uint64_t)(void*)(ptr+(256*i)), 15,         64,      0 );
      WAIT_CACHEOPS;
   }

   // Prefetch 64 lines
   for (int j = 0; j < 4; j++)
   {
      prefetch_va(false, to_L1, (uint64_t)(void*)(ptr + (j*256)), 15, 64, 0 );
      WAIT_PREFETCH_0;
   }

   C_TEST_PASS;
}
